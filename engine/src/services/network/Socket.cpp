#include "tomato/services/network/Socket.h"
#include "tomato/Logger.h"

namespace tomato
{
	tomato::Socket::~Socket()
	{
		closesocket(socket_);
	}

	SocketPtr tomato::Socket::CreateSocket()
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


}