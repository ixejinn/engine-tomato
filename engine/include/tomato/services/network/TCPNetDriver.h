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
		~TCPNetDriver();

		bool InitTCPSocket();
		void SendPacket();
		bool RecvPacket();

		const TCPSocketPtr GetSocket() const { return socket_; }
	private:
		TCPSocketPtr socket_;
	};

}

#endif // !TOMATO_TCP_NETDRIVER_H