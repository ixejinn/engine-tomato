#ifndef NETWORK_SERVICE_H
#define NETWORK_SERVICE_H

#include <tomato/services/network/NetDriver.h>
#include <tomato/containers/SPSCQueue.h>

struct MatchRequest;

class SessionManager;
class MatchManager;

class NetworkService
{
public:
	NetworkService(SessionManager&, MatchManager&);
	
	void SendMessage(uint32_t messageType);
	void ProcessPendingPacket();
	void OnPacket();

	void HandleMatchResult();
	void NetRecvThreadLoop();

private:
	tomato::NetDriver driver_;
	SessionManager& sessionMgr_;
	MatchManager& matchMgr_;
};

#endif // !NETWORK_SERVICE_H