#include "tomato/services/network/NetworkService.h"
#include "tomato/Logger.h"

namespace tomato
{
	namespace
	{
		const int MAX_PLAYER = 4;
	}

	NetworkService::NetworkService() : playerId_(0) {}
	
	bool NetworkService::InitSocket()
	{
		socket_ = Socket::CreateSocket();

		uint32_t port = 7777;
		SocketAddress myAddr(INADDR_ANY, port);
		socket_->Bind(myAddr);

		TMT_LOG << "Initializing NetworkService at port " << port;

		if (socket_ == nullptr)
			return false;

		if (socket_->SetNonBlockingMode(true) != NO_ERROR)
			return false;

		return true;
	}


}