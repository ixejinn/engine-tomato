#include "SessionManager.h"

void SessionManager::Update()
{
	ServerTimeMs now = duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	
	for (auto it = sessions.begin(); it != sessions.end();)
	{
		if (it->second.IsTimeOut(now))
			it = sessions.erase(it);
		else
			++it;
	}
}

void SessionManager::GenerateSession(const tomato::SocketAddress& addr)
{
	SessionId newId = nextId_++;
	sessions.try_emplace(newId, newId, addr, duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
	addrToSessionId[addr] = newId;
}

void SessionManager::GenerateSession(const tomato::TCPSocketPtr client, const tomato::SocketAddress& inAddr)
{
	if (tcpSessions.find(client) == tcpSessions.end())
		tcpSessions.try_emplace(client, client, inAddr);
}

bool SessionManager::ValidateSession(const tomato::SocketAddress& addr)
{
	return addrToSessionId.find(addr) != addrToSessionId.end();
}

bool SessionManager::ValidateSession(const SessionId id)
{
	return sessions.find(id) != sessions.end();
}

bool SessionManager::ValidateSession(const tomato::TCPSocketPtr client)
{
	return tcpSessions.find(client) != tcpSessions.end();
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

bool SessionManager::RemoveSession(const tomato::TCPSocketPtr client)
{
	auto it = tcpSessions.find(client);
	if(it == tcpSessions.end())
		return false;

	tcpSessions.erase(client);
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

bool SessionManager::AppendRecvBuffer(tomato::TCPSocketPtr client, const uint8_t* inData, int len, std::vector<uint8_t>& outData)
{
	auto it = tcpSessions.find(client);
	if (it != tcpSessions.end())
	{
		it->second.AppendRecvBuffer(inData, len);
		return it->second.ParsePacket(outData);
	}
	return false;
}