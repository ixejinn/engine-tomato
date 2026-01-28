#ifndef TOMATO_TCP_SOCKET_H
#define TOMATO_TCP_SOCKET_H

#include "tomato/services/network/SocketUtil.h"
#include <WinSock2.h>
#include <memory>
#include <typeinfo>

namespace tomato
{
	class SocketAddress;
	class TCPSocket;
	typedef std::shared_ptr<TCPSocket> TCPSocketPtr;

	class TCPSocket
	{
	public:

		~TCPSocket();

		static TCPSocketPtr CreateTCPSocket();

		int Connect(const SocketAddress& inAddress);
		int Bind(const SocketAddress& inToAddress);
		int Listen(int inBackLog = 32);
		TCPSocketPtr Accept(SocketAddress& inFromAddress);
		int Send(const void* inData, int inLen);
		int Receive(void* inBuffer, int inLen);


	private:
		friend class SocketUtil;
		TCPSocket(SOCKET inSocket) : socket_(inSocket) {};
		SOCKET socket_;
	};
}

#endif // !TOMATO_TCP_SOCKET_H