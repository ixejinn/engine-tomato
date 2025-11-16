#include "tomato/services/network/Socket.h"
#include "tomato/Logger.h"
#include "tomato/services/network/SocketAddress.h"

#include <WS2tcpip.h>

namespace tomato
{

	bool Socket::InitWinsock()
	{
		WSADATA wsaData;
		int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != NO_ERROR)
		{
			TMT_ERR << "Failed to Socket::InitWinsock";
			return false;
		}
		
		return true;
	}

	void Socket::CleanUp()
	{
		WSACleanup();
	}

	SocketPtr Socket::CreateSocket()
	{
		SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (s != INVALID_SOCKET)
			return SocketPtr(new Socket(s));
		else
		{
			TMT_ERR << "Failed to Socket::CreateCocket";
			return nullptr;
		}
	}

	Socket::~Socket()
	{
		closesocket(socket_);
	}

	int Socket::Bind(const SocketAddress& inBindAddress)
	{
		int error = bind(socket_, &inBindAddress.sockAddr_, inBindAddress.GetSize());
		if (error != 0)
		{
			TMT_ERR << "Failed to Socket::Bind";
			return WSAGetLastError();
		}

		return NO_ERROR;
	}

	int Socket::SendTo(const void* inToSend, int inLength, const SocketAddress& inToAddress)
	{
		int byteSentCount = sendto(socket_, static_cast<const char*>(inToSend), inLength, 0, &inToAddress.sockAddr_, inToAddress.GetSize());
		
		if (byteSentCount <= 0)
		{
			TMT_ERR << "Failed to Socket::SendTo";
			return -WSAGetLastError();
		}
		else
			return byteSentCount;
	}

	int Socket::ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress)
	{
		socklen_t fromLength = outFromAddress.GetSize();
		int readByteCount = recvfrom(socket_, static_cast<char*>(inToReceive), inMaxLength, 0, &outFromAddress.sockAddr_, &fromLength);
		
		if (readByteCount >= 0)
			return readByteCount;
		else
		{
			int error = WSAGetLastError();

			if (error == WSAEWOULDBLOCK)
				return 0;
			else if (error == WSAECONNRESET)
			{
				TMT_LOG << "Connection reset from " << outFromAddress.ToString();
				return -WSAECONNRESET;
			}
			else
			{
				TMT_ERR << "Failed to Socket::ReceiveFrom";
				return -error;
			}
		}
	}

	int Socket::SetNonBlockingMode(bool nonBlocking)
	{
		u_long arg = nonBlocking ? 1 : 0;
		int result = ioctlsocket(socket_, FIONBIO, &arg);

		if (result == SOCKET_ERROR)
		{
			TMT_ERR << "Failed to Socket::SetNonBlockingMode";
			return WSAGetLastError();
		}
		else
			return NO_ERROR;
	}
}