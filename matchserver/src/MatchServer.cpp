#include "MatchServer.h"

#include <iostream>

MatchServer::MatchServer()
	:
	matchMgr_(MatchRequestQueue, NetSendRequestQueue),
	networkService_(sessionMgr_, matchMgr_, MatchRequestQueue, NetSendRequestQueue)
	{};

void MatchServer::Run()
{

	while (isRunning_)
	{
		std::cout << "run" << '\n';
	}
};