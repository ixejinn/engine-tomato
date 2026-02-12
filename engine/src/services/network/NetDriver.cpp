#include "tomato/services/network/NetDriver.h"
#include "tomato/net/NetBitReader.h"
#include "tomato/net/NetBitWriter.h"
#include "tomato/net/InputNetMessage.h"
#include "tomato/Logger.h"

namespace tomato
{
    NetDriver::NetDriver()
    {
        InitSocket();
    }
    
    NetDriver::~NetDriver(){}

    bool NetDriver::InitSocket()
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

        TMT_INFO << "Initializing UDP NetDriver at " << myAddr.ToString();

        if (socket_ == nullptr)
            return false;

        if (socket_->SetNonBlockingMode(true) != NO_ERROR)
            return false;

        return true;
    }
    
    void NetDriver::SendPacket(uint32_t messageType, const SocketAddress& inToAddress)
	{
        RawBuffer rawBuffer;
        NetBitWriter writer{ &rawBuffer };

        writer.WriteInt(messageType, 4);

        // !!! �׽�Ʈ �ڵ� NetMessageRegistry ����� �����ؾ� �� !!!
        if (messageType == 0)
        {
            InputNetMessage tmp;
            //tmp.Write(writer, engine_);
        }

        socket_->SendTo(rawBuffer.data(), MAX_PACKET_SIZE, inToAddress);
        //std::cout << "NetworkService::SendPacket " << engine_.GetTick() << "\n";
        // !!! �׽�Ʈ �ڵ� NetMessageRegistry ����� �����ؾ� �� !!!
	}

	bool NetDriver::RecvPacket(RawBuffer* buffer, int size, SocketAddress& outFromAddress)
	{
        int receivedBytes = socket_->ReceiveFrom(buffer, MAX_PACKET_SIZE, outFromAddress);
        if (receivedBytes > 0)
            return true;
        
        return false;
	}
}
