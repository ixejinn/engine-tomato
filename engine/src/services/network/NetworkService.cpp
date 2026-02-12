#include "tomato/services/network/NetworkService.h"
#include "tomato/Logger.h"
#include "tomato/Engine.h"
#include "tomato/net/NetBitReader.h"
#include "tomato/net/NetBitWriter.h"
#include "tomato/net/InputNetMessage.h"

namespace tomato
{
	NetworkService::NetworkService(Engine& engine)
    : engine_(engine), playerID_(0)
    {
#if 0
        InitSocket();

        playerToName[0] = "yejin";
        playerToSocket[0] = SocketAddress{"192.168.55.165", 7777};
        socketToPlayer[playerToSocket[0]] = 0;

//        playerToName[1] = "yujung";
//        playerToSocket[1] = SocketAddress{"192.168.31.234", 7777};
//        socketToPlayer[playerToSocket[1]] = 1;

        playerToName[1] = "yejin2";
        playerToSocket[1] = SocketAddress{"192.168.55.88", 7777};
        socketToPlayer[playerToSocket[1]] = 1;
#elif 1
        
        //test code
        ConnectToServer();
        SocketAddress address[4] = { {"192.168.55.165", 0}, {"192.168.31.234", 0},  {"192.168.55.88", 0} };
        conn.try_emplace(0, 0, 0, 0, "yejin", address[0]);
        conn.try_emplace(1, 0, 0, 1, "yejin2", address[2]);

        addToId[address[0]] = 0;
        addToId[address[2]] = 1;
#endif
    }

    NetworkService::~NetworkService()
    {
    }

    // !!! FOR TEST !!!
    void NetworkService::ReadIncomingData()
    {
        //std::cout << "ReadData\n";
        char buffer[512] = { '\0' };
        SocketAddress fromAddr;
        int readByteCount{};

        if (driver_.RecvPacket(buffer, readByteCount, fromAddr))
        {
            buffer[readByteCount] = '\0';
            std::cout << "[" << fromAddr.ToString() << "] : " << buffer << '\n';
        }
    }

    void NetworkService::SendOutgoingData(const SocketAddress& inToAddress)
    {
        //std::cout << "SendData\n";
        //std::string str;
        ////while (true){
        //std::cin >> str;

        //int sentByteCount = socket_->SendTo(str.c_str(), (int)str.size(), inToAddress);
        //std::cout << sentByteCount << std::endl;
        ////}
    }
    // !!! FOR TEST !!!

    void NetworkService::ConnectToServer()
    {
        server_ = TCPSocket::CreateTCPSocket();
        int err = server_->Connect({ "192.168.31.234", 7777 });
        if (err == NO_ERROR)
            std::cout << "Connect to Server\n";
        else if (err == -WSAECONNREFUSED)
            std::cout << "Not activated Match server\n";
        else
            std::cout << err << '\n';
    }

    // use network thread
    void NetworkService::NetThreadLoop()
    {
        SocketAddress fromAddr;
        isNetThreadRunning_ = true;
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

    /*
    void NetworkService::Dispatch()
    {
        SocketAddress fromAddr;

        isNetThreadRunning_ = true;
        while (isNetThreadRunning_)
        {
            RawBuffer* buffer = bufferPool_.Allocate();
            int receivedBytes = socket_->ReceiveFrom(buffer, MAX_PACKET_SIZE, fromAddr);
            //std::cout << "NetworkService::Dispatch() " << engine_.GetTick() << "tick | " << receivedBytes << "\n";

            if (receivedBytes > 0)
            {
                pendingPackets_.Emplace(buffer, receivedBytes, fromAddr);
            }
            else
                bufferPool_.Deallocate(buffer);
        }
    }
    */
    void NetworkService::ProcessPendingPacket()
    {
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

    void NetworkService::TCPNetRecvThreadLoop()
    {
        while (true)
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
            else
            {
                std::cout << received << '\n';
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
            //Set NetConnections
            std::cout << "Receive MATCH_INTRO\n";
            HandleMatchIntroPacket(reader);
            //Send Hello Packet to all peers
            //Send TIME_SYNC_REQ Packet to server
            break;

        case TCPPacketType::TIME_SYNC_RES:
            std::cout << "Receive TIME_SYNC_RES\n";
            //Calculate Server Tick
            //and Send MATCH_INTRO_RESULT(SUCCESS/FAILED) Packet to server
            break;

        case TCPPacketType::MATCH_START:
            std::cout << "Receive MATCH_START\n";
            break;
        }
    }

    void NetworkService::SendTCPPacket(TCPPacketType messageType)
    {
        RawBuffer rawBuffer{};
        NetBitWriter writer{ &rawBuffer };
        writer.WriteInt(static_cast<uint16_t>(0), std::numeric_limits<uint16_t>::max());
        writer.WriteInt(static_cast<uint16_t>(messageType), static_cast<uint16_t>(TCPPacketType::COUNT));

        switch (messageType)
        {
        case TCPPacketType::MATCH_REQUEST:
        {
            writer.WriteInt(static_cast<uint16_t>(name_.size()), std::numeric_limits<uint16_t>::max());
            for (int i = 0; i < name_.size(); i++)
                writer.WriteInt(static_cast<uint8_t>(name_[i]), std::numeric_limits<uint8_t>::max());
            break;
        }

        case TCPPacketType::MATCH_CANCEL:
        case TCPPacketType::MATCH_INTRO_SUCCESS:
        case TCPPacketType::MATCH_INTRO_FAILED:
        {
            writer.WriteInt(matchID_, std::numeric_limits<MatchId>::max());
            break;
        }

        case TCPPacketType::TIME_SYNC_REQ:
            break;
        }
        
        int16_t byteSize = writer.GetByteSize();
        std::memcpy(rawBuffer.data(), &byteSize, sizeof(uint16_t));

        server_->Send(rawBuffer.data(), byteSize);
    }

    void NetworkService::HandleMatchIntroPacket(NetBitReader& reader)
    {
        uint16_t readMatchId{ 0 };
        reader.ReadInt(readMatchId, std::numeric_limits<uint16_t>::max());
        matchID_ = readMatchId;
        std::cout << "[MATCH ID = " << matchID_ << "]\n";
        for (int i = 0; i < 1; i++)
        {
            uint8_t readPlayerId{ 0 }, rname{};
            uint16_t readNameSize{};
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
            tomato::SocketAddress readAddr(addr, 0);

            //std::cout << int(readSize) << " " << int(readType) << " " << int(readMatchId) << " " << int(readPlayerId) << '\n';
            //std::cout << readName << " -> "  << readAddr.ToString() << '\n';

            conn.try_emplace(readPlayerId, 0, readMatchId, readPlayerId, readName, readAddr);
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