#ifndef MATCH_SERVER_H
#define MATCH_SERVER_H

#include "SessionManager.h"
#include "MatchManager.h"
#include "NetworkService.h"

#include <tomato/containers/SPSCQueue.h>

class MatchServer
{
public:
	MatchServer();
	void Run();

private:
	bool isRunning_{ true };

	SessionManager sessionMgr_;
	MatchManager matchMgr_;
	NetworkService networkService_;

	tomato::SPSCQueue<SendRequestCommand, 256> NetSendRequestQueue;
	tomato::SPSCQueue<MatchRequestCommand, 128> MatchRequestQueue;
};

#endif // !MATCH_SERVER_H