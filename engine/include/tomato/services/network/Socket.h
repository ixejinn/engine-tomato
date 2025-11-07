#ifndef TOMATO_SOCKET_H
#define TOMATO_SOCKET_H

#include <WinSock2.h>
#include <memory>

namespace tomato
{
	class SocketAddress;
	class Socket;
	typedef std::shared_ptr<Socket> SocketPtr;

	class Socket
	{
	public:
		static SocketPtr CreateSocket();

		~Socket();

		bool InitWinsock();
		void CleanUp();

		int Bind(const SocketAddress& inBindAddress);
		int SendTo(const void* inToSend, int inLength, const SocketAddress& inToAddress);
		int ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress);

		int SetNonBlockingMode(bool nonBlocking);

	private:
		Socket(SOCKET inSocket) : socket_(inSocket) {};
		SOCKET socket_;
	};

}

#endif // !TOMATO_SOCKET_H