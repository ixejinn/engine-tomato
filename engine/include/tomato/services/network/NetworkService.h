#ifndef TOMATO_NETWORKSERVICE_H
#define TOMATO_NETWORKSERVICE_H

#include "tomato/services/network/Socket.h"
#include "tomato/services/network/SocketAddress.h"

#include <map>
#include <unordered_map>
#include <string>

namespace tomato
{
	class NetworkService
	{
	public:

		NetworkService();

		void ReadIncomingData();
		void SendOutgoingData(const SocketAddress& inToAddress);

		bool InitSocket();
	private:


		std::map<uint32_t, std::string> playerToName;
		std::map<uint32_t, SocketAddress> playerToSocket;
		std::unordered_map<SocketAddress, uint32_t> socketToPlayer;

		SocketPtr socket_;
		std::string name_;
		uint32_t playerId_;
	};
}

#endif // !TOMATO_NETWORKSERVICE_H