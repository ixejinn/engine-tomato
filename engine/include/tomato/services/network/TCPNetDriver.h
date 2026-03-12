#ifndef TOMATO_TCP_NETDRIVER_H
#define TOMATO_TCP_NETDRIVER_H

#include "tomato/services/network/TCPSocket.h"
#include "tomato/services/network/SocketAddress.h"
#include "tomato/services/network/CoreNetwork.h"

namespace tomato
{
	enum class NetMode
	{
		NM_ListenServer,
		NM_Client
	};
	class TCPNetDriver
	{
	public:
		explicit TCPNetDriver(NetMode mode);
		~TCPNetDriver();

		bool InitListenServerMode();
		bool InitClientMode();

		int SendPacket(uint8_t* buffer, int size);
		int RecvPacket(uint8_t* buffer, int size);

		const TCPSocketPtr GetSocket() const { return socket_; }

		bool IsConnectedToServer() const { return connectedToServer; }
	private:
		
		NetMode mode_;
		TCPSocketPtr socket_;

		bool connectedToServer = false;
	};

}

#endif // !TOMATO_TCP_NETDRIVER_H