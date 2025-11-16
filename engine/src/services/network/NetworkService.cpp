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
		SocketAddress myAddr((uint32_t)INADDR_ANY, port);
		socket_->Bind(myAddr);

		TMT_LOG << "Initializing NetworkService at " << myAddr.ToString();

		if (socket_ == nullptr)
			return false;

		if (socket_->SetNonBlockingMode(true) != NO_ERROR)
			return false;

		return true;
	}

	void NetworkService::ReadIncomingData()
	{
		//std::cout << "ReadData\n";
		char buffer[512] = { '\0' };
		SocketAddress fromAddr;

		int readByteCount = socket_->ReceiveFrom(buffer, sizeof(buffer) - 1, fromAddr);
		
		if (readByteCount == 0)
			return;
		else if (readByteCount > 0)
		{
			buffer[readByteCount] = '\0';
			std::cout << "[" << fromAddr.ToString() << "] : " << buffer << '\n';
		}
	}

	void NetworkService::SendOutgoingData(const SocketAddress& inToAddress)
	{
		std::cout << "SendData\n";
		std::string str;
		//while (true){
			std::cin >> str;

			int sentByteCount = socket_->SendTo(str.c_str(), (int)str.size(), inToAddress);
			std::cout << sentByteCount << std::endl;
		//}
	}
}