#include "tomato/services/network/NetworkService.h"
#include "tomato/Logger.h"
#include "tomato/Engine.h"
#include "tomato/net/NetBitReader.h"
#include "tomato/net/NetBitWriter.h"
#include "tomato/net/InputNetMessage.h"

namespace tomato
{
	NetworkService::NetworkService(Engine& engine)
    : engine_(engine), playerID_(0), netState_(NetworkServiceState::NSS_Uninitialized)
    {
        //test code
        ConnectToServer();
        isNetThreadRunning_ = true;
    }

    NetworkService::~NetworkService() {}

    void NetworkService::ConnectToServer()
    {
        server_ = TCPSocket::CreateTCPSocket();
        int err = server_->Connect({ "192.168.45.239", 7777 });

        if (err == NO_ERROR)
            std::cout << "Connect to Server\n";
        else if (err == -WSAECONNREFUSED)
            std::cout << "Not activated Match server\n";
        else
            std::cout << err << '\n';

        TCPRecvThreadRunning_ = true;
    }

    // use network thread
    void NetworkService::NetThreadLoop()
    {
        SocketAddress fromAddr;
        while (isNetThreadRunning_)
        {
            RawBuffer* buffer = bufferPool_.Allocate();
            int receivedBytes;
            if (driver_.RecvPacket(buffer, receivedBytes, fromAddr))
                pendingPackets_.Emplace(buffer, receivedBytes, fromAddr);
            else
                bufferPool_.Deallocate(buffer);
        }
    }

    void NetworkService::ProcessPendingPacket()
    {
        /*
        // !!! 테스트 코드 NetMessageRegistry 만들면 수정해야 함 !!!
        while (!pendingPackets_.Empty())
        {
            Packet packet;
            pendingPackets_.Dequeue(packet);
            NetBitReader reader(packet.buffer->data(), packet.size);

            uint32_t messageType = -1;
            reader.ReadInt(messageType, 4);   // 예비로 비트 2개만 확인하게 함

            if (messageType == 0)
            {
                InputNetMessage tmp;
                tmp.Read(reader, engine_, packet.addr);
                //std::cout << "NetworkService::ProcessPendingPacket() " << engine_.GetTick() << "\n";
            }

            bufferPool_.Deallocate(packet.buffer);
        }
        // !!! 테스트 코드 NetMessageRegistry 만들면 수정해야 함 !!!
        */
    }

    void NetworkService::ProcessQueuedUDPPacket()
    {
        while (!pendingPackets_.Empty())
        {
            Packet packet;
            pendingPackets_.Dequeue(packet);
            NetBitReader reader(packet.buffer->data(), packet.size);

            uint16_t type;
            reader.ReadInt(type, static_cast<uint16_t>(UDPPacketType::COUNT));

            ProcessUDPPacket(static_cast<UDPPacketType>(type), reader, packet.addr);

            bufferPool_.Deallocate(packet.buffer);
        }
    }

    void NetworkService::ProcessUDPPacket(const UDPPacketType& type, NetBitReader& reader, const SocketAddress& inToAddress)
    {
        switch (type)
        {
        case UDPPacketType::HELLO:
            //Send Welcome to peer
            std::cout << "Receive HELLO\n";
            SendUDPPacket(UDPPacketType::WELCOME, SendPolicy::Unicast, &inToAddress);
            break;

        case UDPPacketType::WELCOME:
            //Check the peer is connected
            std::cout << "Receive WELCOME\n";
            if (HandleWelcomePacket(inToAddress))
            {
                std::cout << "All Checked\n";
                SendTCPPacket(TCPPacketType::MATCH_INTRO_SUCCESS);
                netState_ = NetworkServiceState::NSS_Lobby;
            }
            //TODO@ timeout
            break;

        case UDPPacketType::INPUT:
        {
            InputNetMessage tmp;
            tmp.Read(reader, engine_, inToAddress);
            break;
        }
        }

    }

    void NetworkService::BuildUDPPacket(NetBitWriter& writer, UDPPacketType messageType)
    {
        writer.WriteInt(static_cast<uint16_t>(messageType), static_cast<uint16_t>(UDPPacketType::COUNT));

        switch (messageType)
        {
        case UDPPacketType::HELLO:
            break;

        case UDPPacketType::WELCOME:
            break;

        case UDPPacketType::INPUT:
        {
            InputNetMessage tmp;
            tmp.Write(writer, engine_);
            break;
        }
        }
    }

    void NetworkService::BroadcastToPeers(const void* buffer)
    {
        for (auto it = conn.begin(); it != conn.end(); it++)
        {
            if (it->second.playerId == playerID_)
                continue;

            int byteSentCount{};
            driver_.SendPacket(buffer, byteSentCount, it->second.addr);
        }
    }

    void NetworkService::SendUDPPacket(UDPPacketType messageType, SendPolicy policy, const SocketAddress* inToAddress)
    {
        RawBuffer rawBuffer{};
        NetBitWriter writer{ &rawBuffer };

        BuildUDPPacket(writer, messageType);

        if (inToAddress)
        {
            int byteSentCount{};
            driver_.SendPacket(rawBuffer.data(), byteSentCount, *inToAddress); 
        }
        
        else
            BroadcastToPeers(rawBuffer.data());
    }

    bool NetworkService::HandleWelcomePacket(const SocketAddress& inToAddress)
    {
        PlayerId id = GetPlayerID(inToAddress);
        peerConnected[id] = true;
        
        for (const auto& peer : peerConnected)
        {
            if (!peer)
                return false;
        }

        return true;
    }

    void NetworkService::TCPNetRecvThreadLoop()
    {
        while (TCPRecvThreadRunning_)
        {
            uint8_t segment[MAX_PACKET_SIZE]{};
            int received = server_->Receive(segment, MAX_PACKET_SIZE);
            if (received > 0) //parsing
            {
                recvBuffer.insert(recvBuffer.end(), segment, segment + received);
                if (recvBuffer.size() < sizeof(uint16_t))
                    continue;

                uint16_t size;
                std::memcpy(&size, recvBuffer.data(), sizeof(size));

                if (recvBuffer.size() < size)
                    continue;

                auto packet = std::make_unique<TCPPacket>(size, 0);
                std::memcpy(packet->buffer.data(), recvBuffer.data() + sizeof(uint16_t), size);

                recvBuffer.erase(recvBuffer.begin(), recvBuffer.begin() + size);

                pendingTCPPackets_.Emplace(std::move(packet));
            }
        }
    }

    void NetworkService::ProcessQueuedTCPPacket()
    {
        while (!pendingTCPPackets_.Empty())
        {
            std::unique_ptr<TCPPacket> nextPacket;
            pendingTCPPackets_.Dequeue(nextPacket);

            uint16_t type;
            NetBitReader reader(nextPacket->buffer.data(), nextPacket->size());

            reader.ReadInt(type, static_cast<uint16_t>(TCPPacketType::COUNT));

            ProcessTCPPacket(static_cast<TCPPacketType>(type), reader);
        }
    }

    void NetworkService::ProcessTCPPacket(const TCPPacketType& header, NetBitReader& reader)
    {
        switch (header)
        {
        case TCPPacketType::MATCH_INTRO:
            netState_ = NetworkServiceState::NSS_Hello;
            //Set NetConnections
            std::cout << "Receive MATCH_INTRO\n";
            HandleMatchIntroPacket(reader);
            //Send Hello Packet to all peers
            SendUDPPacket(UDPPacketType::HELLO, SendPolicy::Broadcast);
            //Send TIME_SYNC_REQ Packet to server
            SendTCPPacket(TCPPacketType::TIME_SYNC_REQ);
            break;

        case TCPPacketType::TIME_SYNC_RES:
            std::cout << "Receive TIME_SYNC_RES\n";
            //Calculate Server Tick
            HandleServerTimeSyncPacket(reader);
            break;

        case TCPPacketType::MATCH_START:
            std::cout << "Receive MATCH_START\n";
            HandleMatchStartPacket(reader);
            break;
        }
    }

    void NetworkService::SendTCPPacket(TCPPacketType messageType)
    {
        std::cout << "Send ";

        RawBuffer rawBuffer{};
        NetBitWriter writer{ &rawBuffer };
        writer.WriteInt(static_cast<uint16_t>(0), std::numeric_limits<uint16_t>::max());
        writer.WriteInt(static_cast<uint16_t>(messageType), static_cast<uint16_t>(TCPPacketType::COUNT));

        switch (messageType)
        {
        case TCPPacketType::MATCH_REQUEST:
        {
            // name size + name + udp port number
            writer.WriteInt(static_cast<uint16_t>(name_.size()), std::numeric_limits<uint16_t>::max());
            for (int i = 0; i < name_.size(); i++)
                writer.WriteInt(static_cast<uint8_t>(name_[i]), std::numeric_limits<uint8_t>::max());
            writer.WriteInt(static_cast<uint16_t>(driver_.GetPort()), std::numeric_limits<uint16_t>::max());
            
            break;
        }

        case TCPPacketType::MATCH_CANCEL:
        case TCPPacketType::MATCH_INTRO_SUCCESS:
        case TCPPacketType::MATCH_INTRO_FAILED:
        {
            std::cout << "MATCH_INTRO_SUCCESS\n";
            writer.WriteInt(matchID_, std::numeric_limits<MatchId>::max());
            break;
        }

        case TCPPacketType::TIME_SYNC_REQ:
            std::cout << "MATCH_INTRO_SUCCESS\n";
            sendTime = static_cast<ServerTimeMs>(
                duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now().time_since_epoch()).count());
            break;
        }
        
        int16_t byteSize = writer.GetByteSize();
        std::memcpy(rawBuffer.data(), &byteSize, sizeof(uint16_t));

        server_->Send(rawBuffer.data(), byteSize);  
    }

    void NetworkService::HandleMatchIntroPacket(NetBitReader& reader)
    {
        uint8_t readMyId{ 0 }, readPlayerNum{ 0 };
        uint16_t readMatchId{ 0 };

        reader.ReadInt(readMatchId, std::numeric_limits<uint16_t>::max());
        matchID_ = readMatchId;
        reader.ReadInt(readPlayerNum, std::numeric_limits<uint8_t>::max());
        peerConnected.resize(readPlayerNum);

        std::cout << "[MATCH ID = " << matchID_ << "]\n";
        for (int i = 0; i < readPlayerNum; i++)
        {
            uint8_t readPlayerId{ 0 }, rname{};
            uint16_t readNameSize{}, readPort{};
            std::string readName{};
            uint32_t addr{ 0 };

            reader.ReadInt(readPlayerId, std::numeric_limits<uint8_t>::max());
            reader.ReadInt(readNameSize, std::numeric_limits<uint16_t>::max());

            for (int j = 0; j < readNameSize; j++)
            {
                reader.ReadInt(rname, std::numeric_limits<uint8_t>::max());
                readName += rname;
            }
            reader.ReadInt(addr, std::numeric_limits<uint32_t>::max());
            reader.ReadInt(readPort, std::numeric_limits<uint16_t>::max());

            tomato::SocketAddress readAddr(addr, readPort);

            conn.try_emplace(readPlayerId, 0, readMatchId, readPlayerId, readName, readAddr);
            addToId.try_emplace(readAddr, readPlayerId);
        }

        reader.ReadInt(readMyId, std::numeric_limits<uint8_t>::max());

        playerID_ = readMyId;
        std::cout << "[My PlayerID = " << int(playerID_) << "]\n";
        peerConnected[playerID_] = true;
        //@TODO : if playerId != 0
    }

    void NetworkService::HandleServerTimeSyncPacket(NetBitReader& reader)
    {
        recvTime = static_cast<ServerTimeMs>(
            duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch()).count());

        ServerTimeMs serverSteady{};
        reader.ReadInt(serverSteady, std::numeric_limits<ServerTimeMs>::max());

        //@TODO : Calculate server steady time
        ServerTimeMs rtt = recvTime - sendTime;
        serverTimeOffset = serverSteady - (sendTime + (rtt / 2));

        //estimatedServerTime = localSteadyNow + offset
        ServerTimeMs estimatedServerTime = static_cast<ServerTimeMs>(
            duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch()).count()) + serverTimeOffset;

        estimatedServerTick = floor(estimatedServerTime / 16.67f);
        engine_.SetServerTicks(estimatedServerTick);

        //std::cout << "[Server Tick] " << int(estimatedServerTick) << "\n[Local Tick] " << int(engine_.GetTick()) << '\n';
    }

    void NetworkService::HandleMatchStartPacket(NetBitReader& reader)
    {
        if (netState_ == NetworkServiceState::NSS_Lobby)
        {
            ServerTimeMs serverStartTime{};
            reader.ReadInt(serverStartTime, std::numeric_limits<ServerTimeMs>::max());

            estimatedStartTick = floor(serverStartTime / 16.67f );
            engine_.SetStartTicks(estimatedStartTick);

            //std::cout << "[Start Tick] " << int(estimatedStartTick) << '\n';
            //@TODO : Calculate server tick, inform start tick to engine
            ServerTimeMs localSteadyTimeNow = static_cast<ServerTimeMs>(
                duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now().time_since_epoch()).count());

            ServerTimeMs estimatedServerNow = localSteadyTimeNow + serverTimeOffset;
            ServerTimeMs remaining = serverStartTime - estimatedServerNow;

            //@TODO
            if (remaining < 0)
            {
                std::cout << "Invalid StartTime\n";
                return;
            }
            localStartTime = static_cast<ServerTimeMs>(
                duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now().time_since_epoch()).count()) + remaining;

            engine_.SetStartTimes(localStartTime);
            std::cout << "[Now] " << localSteadyTimeNow << "\n[Start Time] " << localStartTime << '\n';
            netState_ = NetworkServiceState::NSS_Starting;
        }
    }

//    void NetworkService::SendPacket(uint32_t messageType)
//    {
//        //RawBuffer* rawBuffer = bufferPool_.Allocate();
//        RawBuffer rawBuffer;
//        NetBitWriter writer{&rawBuffer};
//
//        writer.WriteInt(messageType, 4);
//
//        // !!! 테스트 코드 NetMessageRegistry 만들면 수정해야 함 !!!
//        if (messageType == 0)
//        {
//            InputNetMessage tmp;
//            tmp.Write(writer, engine_);
//        }
//
//#if 0
//        //std::cout << "NetworkService::SendPacket " << engine_.GetTick() << "\n";
//        for (auto& playerAddr : playerToSocket)
//        {
//            if (playerAddr.first == playerID_)
//                continue;
//            socket_->SendTo(rawBuffer.data(), MAX_PACKET_SIZE, playerAddr.second);
//            //std::cout << "NetworkService::SendPacket " << engine_.GetTick() << "\n";
//        }
//        // !!! 테스트 코드 NetMessageRegistry 만들면 수정해야 함 !!!
//
////        bufferPool_.Deallocate(rawBuffer);
//#elif 1
//        for (auto it = conn.begin(); it != conn.end(); it++)
//        {
//            if (it->second.playerId == playerID_)
//                continue;
//
//            driver_.SendPacket(messageType, it->second.addr);
//        }
//#endif
//    }
}