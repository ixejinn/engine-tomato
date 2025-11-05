#ifndef TOMATO_SOCKET_H
#define TOMATO_SOCKET_H

#include <WinSock2.h>
#include <memory>

#include "SocketAddress.h"

namespace tomato
{
	class Socket
	{
	public:
		~Socket();

		//bool InitSocket();
		SocketPtr CreateSocket();

		int Bind(const SocketAddress& inBindAddress);
		int SendTo(const void* inToSend, int inLength, const sockaddr& inToAddress);
		int ReceiveFrom(void* inToReceive, int inMaxLength, sockaddr& outFromAddress);

	private:
		Socket(SOCKET inSocket) : socket_(inSocket) {};
		SOCKET socket_;
	};

	typedef std::shared_ptr<Socket> SocketPtr;
}

#endif // !TOMATO_SOCKET_H