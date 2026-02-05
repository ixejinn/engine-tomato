#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <chrono>
#include <unordered_map>
#include <memory>
#include "Session.h"
#include "PacketTypes.h"

class SessionManager
{
public:
	void Update();

	// For TCP
	void GenerateSession(const tomato::TCPSocketPtr& client, const tomato::SocketAddress& inAddr); 
	bool ValidateSession(const SessionId& id);
	bool ValidateSession(const tomato::TCPSocketPtr& client);
	bool RemoveSession(const tomato::TCPSocketPtr& client);
	std::unique_ptr<TCPPacket> AppendRecvBuffer(const SessionId& client, const uint8_t* inData, const int& len);
	void AppendSendBuffer(const SessionId& client, const uint8_t* inData, const int& len);

	void GetWritableSockets(std::vector<tomato::TCPSocketPtr>& outVector);
	SessionId GetSessionId(const tomato::TCPSocketPtr& client) { return socketToId[client]; }
	TCP::Session* GetSession(const tomato::TCPSocketPtr& socket);

	// For UDP
	void GenerateSession(const tomato::SocketAddress&); 
	bool ValidateSession(const tomato::SocketAddress&);
	bool RemoveSession(SessionId&);
	void UpdateLastRecv(const SessionId&);

	SessionId GetSessionId(const tomato::SocketAddress& addr) { return addrToSessionId[addr]; }
private:
	std::unordered_map<SessionId, TCP::Session> tcpSessions;
	std::unordered_map<tomato::TCPSocketPtr, SessionId> socketToId;

	SessionId nextId_ = 1;
	std::unordered_map<SessionId, Session> sessions;
	std::unordered_map<tomato::SocketAddress, SessionId> addrToSessionId;
};

#endif // !SESSION_MANAGER_H