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

	TCPNetDriver::TCPNetDriver(SocketAddress& inAddress)
	{
		if (inAddress.GetIPv4() == (uint32_t)INADDR_ANY)
			InitTCPSocket();
		else
			socket_->Accept(inAddress);
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
			TMT_INFO << "Failed to Initialize TCP NetDriver";
			return false;
		}

		TMT_INFO << "Initializing TCP NetDriver at " << myAddr.ToString();
		socket_->Listen();
		TMT_INFO << "START TO LISTEN....";
		return true;
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