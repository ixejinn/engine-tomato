#ifndef MATCH_SERVER_H
#define MATCH_SERVER_H

class SessionManager;
class MatchManager;
class NetworkService;

class MatchServer
{
public:
	MatchServer();
	void Run();

private:
	bool isRunning_{ true };

	SessionManager* sessionMgr_{ nullptr };
	MatchManager* matchMgr_{ nullptr };
	NetworkService* networkService_{ nullptr };
};

#endif // !MATCH_SERVER_H