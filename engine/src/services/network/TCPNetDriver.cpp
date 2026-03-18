#include "tomato/services/network/TCPNetDriver.h"
#include "tomato/net/NetBitReader.h"
#include "tomato/net/NetBitWriter.h"
#include "tomato/net/InputNetMessage.h"
#include "tomato/Logger.h"
#include "tomato/EngineConfig.h"

namespace tomato
{
	TCPNetDriver::TCPNetDriver(NetMode mode) : mode_(mode)
	{
		if (mode_ == NetMode::NM_ListenServer)
			InitListenServerMode();

		else if (mode_ == NetMode::NM_Client)
			InitClientMode();
	}

	TCPNetDriver::~TCPNetDriver(){}

	bool TCPNetDriver::InitListenServerMode()
	{
		socket_ = TCPSocket::CreateTCPSocket();

		uint32_t port = 7777;
		SocketAddress myAddr((uint32_t)INADDR_ANY, port);
		socket_->Bind(myAddr);
		
		if (socket_ == nullptr)
		{
			TMT_INFO << "Failed to Initialize TCP NetDriver";
			return false;
		}

		TMT_INFO << "Initializing TCP NetDriver at " << myAddr.ToString();
		socket_->Listen();

		TMT_INFO << "START TO LISTEN....";

		return true;
	}

	bool TCPNetDriver::InitClientMode()
	{
		socket_ = TCPSocket::CreateTCPSocket();
		int err = socket_->Connect({ EngineConfig::SERVER_ADDRESS, EngineConfig::PORT_NUM });

		if (err == NO_ERROR)
		{
			std::cout << "Connect to Server\n";
			connectedToServer = true;

			return true;
		}

		else if (err == -WSAECONNREFUSED)
			std::cout << "Not activated Match server\n";

		else
			std::cout << err << '\n';

		return false;
	}

	int TCPNetDriver::SendPacket(uint8_t* buffer, int size)
	{
		return socket_->Send(buffer, size);
	}

	int TCPNetDriver::RecvPacket(uint8_t* buffer, int size)
	{
		return socket_->Receive(buffer, size);
	}

}