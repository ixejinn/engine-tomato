#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <chrono>
#include <unordered_map>
#include <Session.h>

class SessionManager
{
public:
	void Update();

	void GenerateSession(const tomato::SocketAddress);
	bool ValidateSession(const SessionId);
	bool ValidateSession(const tomato::SocketAddress);
	bool RemoveSession(SessionId);
	void UpdateLastRecv(const SessionId);

	SessionId GetSessionId(const tomato::SocketAddress& addr) { return addrToSessionId[addr]; }
private:
	SessionId nextId_ = 1;

	std::unordered_map<SessionId, Session> sessions;
	std::unordered_map<tomato::SocketAddress, SessionId> addrToSessionId;
};

#endif // !SESSION_MANAGER_H