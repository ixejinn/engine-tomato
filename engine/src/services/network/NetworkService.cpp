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
        Socket::InitWinsock();
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
    }

    NetworkService::~NetworkService()
    {
        Socket::CleanUp();
    }

    // !!! FOR TEST !!!
    void NetworkService::ReadIncomingData()
    {
        //std::cout << "ReadData\n";
        char buffer[512] = { '\0' };
        SocketAddress fromAddr;

        int readByteCount = socket_->ReceiveFrom(buffer, sizeof(buffer) - 1, fromAddr);

        if (readByteCount == 0)
            return;
        else if (readByteCount > 0)
        {
            buffer[readByteCount] = '\0';
            std::cout << "[" << fromAddr.ToString() << "] : " << buffer << '\n';
        }
    }

    void NetworkService::SendOutgoingData(const SocketAddress& inToAddress)
    {
        std::cout << "SendData\n";
        std::string str;
        //while (true){
        std::cin >> str;

        int sentByteCount = socket_->SendTo(str.c_str(), (int)str.size(), inToAddress);
        std::cout << sentByteCount << std::endl;
        //}
    }
    // !!! FOR TEST !!!
	
	bool NetworkService::InitSocket()
	{
		/**
        * Create UDP socket for game networking.
        * If this fails, network service cannot start.
        */
        socket_ = Socket::CreateSocket();

        // INADDR_ANY allows receiving packets from any Network Interface Controller.
		uint32_t port = 7777;
		SocketAddress myAddr((uint32_t)INADDR_ANY, port);
		socket_->Bind(myAddr);

		TMT_LOG << "Initializing NetworkService at " << myAddr.ToString();

		if (socket_ == nullptr)
			return false;

		if (socket_->SetNonBlockingMode(true) != NO_ERROR)
			return false;

		return true;
	}

    // use network thread
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

    void NetworkService::SendPacket(uint32_t messageType)
    {
        //RawBuffer* rawBuffer = bufferPool_.Allocate();
        RawBuffer rawBuffer;
        NetBitWriter writer{&rawBuffer};

        writer.WriteInt(messageType, 4);

        // !!! 테스트 코드 NetMessageRegistry 만들면 수정해야 함 !!!
        if (messageType == 0)
        {
            InputNetMessage tmp;
            tmp.Write(writer, engine_);
        }

        //std::cout << "NetworkService::SendPacket " << engine_.GetTick() << "\n";
        for (auto& playerAddr : playerToSocket)
        {
            if (playerAddr.first == playerID_)
                continue;
            socket_->SendTo(rawBuffer.data(), MAX_PACKET_SIZE, playerAddr.second);
            //std::cout << "NetworkService::SendPacket " << engine_.GetTick() << "\n";
        }
        // !!! 테스트 코드 NetMessageRegistry 만들면 수정해야 함 !!!

//        bufferPool_.Deallocate(rawBuffer);
    }
}