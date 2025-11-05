#include "tomato/services/network/Socket.h"
#include "tomato/Logger.h"

namespace tomato
{
	Socket::~Socket()
	{
		closesocket(socket_);
	}

	SocketPtr Socket::CreateSocket()
	{
		socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (socket_ != INVALID_SOCKET)
			return SocketPtr(new Socket(socket_));
		else
		{
			TMT_ERR << "Failed to create socket";
			return nullptr;
		}
	}

	int Socket::Bind(const SocketAddress& inBindAddress)
	{
		int error = bind(socket_, &inBindAddress.sockAddr_, inBindAddress.GetSize());
		if (error != 0)
		{
			TMT_ERR << "Failed to bind socket";
			return WSAGetLastError();
		}

		return NO_ERROR;
	}

	int Socket::SendTo(const void* inToSend, int inLength, const sockaddr& inToAddress)
	{
		return 0;
	}

	int Socket::ReceiveFrom(void* inToReceive, int inMaxLength, sockaddr& outFromAddress)
	{
		return 0;
	}
}