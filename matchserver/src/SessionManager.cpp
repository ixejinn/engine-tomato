#include "SessionManager.h"

void SessionManager::Update()
{
	ServerTimeMs now = duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	auto it = sessions.begin();
	for (auto it = sessions.begin(); it != sessions.end();)
	{
		if (it->second.IsTimeOut(now))
			it = sessions.erase(it);
		else
			++it;
	}
}

void SessionManager::GenerateSession(const tomato::SocketAddress addr)
{
	SessionId newId = nextId_++;
	sessions.try_emplace(newId, newId, addr, duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
	addrToSessionId[addr] = newId;
}

bool SessionManager::ValidateSession(const tomato::SocketAddress addr)
{
	return addrToSessionId.find(addr) != addrToSessionId.end();
}

bool SessionManager::ValidateSession(const SessionId id)
{
	return sessions.find(id) != sessions.end();
}

bool SessionManager::RemoveSession(SessionId id)
{
	auto it = sessions.find(id);
	if(it == sessions.end())
		return false;

	addrToSessionId.erase(it->second.GetSessionAddr());
	sessions.erase(id);

	return true;
}

void SessionManager::UpdateLastRecv(const SessionId id)
{
	auto it = sessions.find(id);
	if (it == sessions.end())
		return;

	ServerTimeMs now = duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	it->second.UpdateLastRecv(now);
}