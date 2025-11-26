#include "tomato/services/network/NetworkService.h"
#include "tomato/Logger.h"
#include "tomato/Engine.h"
#include "tomato/net/NetBitReader.h"
#include "tomato/net/NetBitWriter.h"

namespace tomato
{
	NetworkService::NetworkService(Engine& engine)
    : engine_(engine), playerID_(0) {}

    NetworkService::~NetworkService() = default;

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
		socket_ = Socket::CreateSocket();

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

            if (receivedBytes > 0)
            {
                //pendingPackets_.replace(buffer, receivedBytes, fromAddr);
            }
            else
                bufferPool_.Deallocate(buffer);
        }
    }

    void NetworkService::ProcessPendingPacket()
    {
        // 큐 반복문에 넣을 부분
        // PacketHeader 필요
        //Packet& packet = pendingPackets_.front();
        //NetBitReader reader(packet.buffer, packet.size);

        uint32_t messageType = -1;
        //reader.ReadInt(messageType, 4);   // 예비로 비트 2개만 확인하게 함

        // !!! 테스트 코드 NetMessageRegistry 만들면 수정해야 함 !!!
        if (messageType == 0)
        {

        }
        // !!! 테스트 코드 NetMessageRegistry 만들면 수정해야 함 !!!
    }

    void NetworkService::SendPacket(uint32_t messageType)
    {
        RawBuffer* rawBuffer = bufferPool_.Allocate();
        NetBitWriter writer{rawBuffer};

        writer.WriteInt(messageType, 4);

        // !!! 테스트 코드 NetMessageRegistry 만들면 수정해야 함 !!!
        if (messageType == 0)
        {

        }

        for (auto& playerAddr : playerToSocket)
        {
            socket_->SendTo(rawBuffer->data(), MAX_PACKET_SIZE, playerAddr.second);
        }
        // !!! 테스트 코드 NetMessageRegistry 만들면 수정해야 함 !!!

        bufferPool_.Deallocate(rawBuffer);
    }
}