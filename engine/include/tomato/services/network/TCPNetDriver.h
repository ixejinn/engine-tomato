#ifndef TOMATO_TCP_NETDRIVER_H
#define TOMATO_TCP_NETDRIVER_H

#include "tomato/services/network/TCPSocket.h"
#include "tomato/services/network/SocketAddress.h"
#include "tomato/services/network/CoreNetwork.h"

namespace tomato
{
	class TCPNetDriver
	{
	public:
		TCPNetDriver();
		TCPNetDriver(SocketAddress& inAddress);
		~TCPNetDriver();

		bool InitTCPSocket();

		int SendPacket(uint8_t* buffer, int size);
		int RecvPacket(uint8_t* buffer, int size);

		//int GetSocketAddress();
		const TCPSocketPtr GetSocket() const { return socket_; }
	private:

		TCPSocketPtr socket_;
	};

}

#endif // !TOMATO_TCP_NETDRIVER_H