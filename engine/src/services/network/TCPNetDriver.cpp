#include "tomato/services/network/TCPNetDriver.h"
#include "tomato/net/NetBitReader.h"
#include "tomato/net/NetBitWriter.h"
#include "tomato/net/InputNetMessage.h"
#include "tomato/Logger.h"


namespace tomato
{
	TCPNetDriver::TCPNetDriver()
	{
		InitTCPSocket();
	}
	TCPNetDriver::~TCPNetDriver()
	{	
	}

	bool TCPNetDriver::InitTCPSocket()
	{
		socket_ = TCPSocket::CreateTCPSocket();

		uint32_t port = 7777;
		SocketAddress myAddr((uint32_t)INADDR_ANY, port);
		socket_->Bind(myAddr);
		if (socket_ == nullptr)
		{
			TMT_LOG << "Failed to Initialize TCP NetDriver";
			return false;
		}

		TMT_LOG << "Initializing TCP NetDriver at " << myAddr.ToString();
		socket_->Listen();
		TMT_LOG << "START TO LISTEN....";
		return true;
	}

}