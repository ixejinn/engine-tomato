#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <chrono>
#include <unordered_map>
#include <Session.h>

class SessionManager
{
public:
	void Update();

	// For TCP
	void GenerateSession(const tomato::TCPSocketPtr& client, const tomato::SocketAddress& inAddr); 
	bool ValidateSession(const tomato::TCPSocketPtr& client);
	bool RemoveSession(const tomato::TCPSocketPtr& client);
	bool AppendRecvBuffer(const tomato::TCPSocketPtr& client, const uint8_t* inData, const int& len, std::vector<uint8_t>& outData);
	void AppendSendBuffer(const tomato::TCPSocketPtr& client, const uint8_t* inData, const int& len);

	void GetWritableSockets(std::vector<tomato::TCPSocketPtr>& outVector);
	TCP::Session* GetSession(const tomato::TCPSocketPtr socket);

	// For UDP
	void GenerateSession(const tomato::SocketAddress&); 
	bool ValidateSession(const SessionId);
	bool ValidateSession(const tomato::SocketAddress&);
	bool RemoveSession(SessionId);
	void UpdateLastRecv(const SessionId);

	SessionId GetSessionId(const tomato::SocketAddress& addr) { return addrToSessionId[addr]; }
private:
	std::unordered_map<tomato::TCPSocketPtr, TCP::Session> tcpSessions;

	SessionId nextId_ = 1;
	std::unordered_map<SessionId, Session> sessions;
	std::unordered_map<tomato::SocketAddress, SessionId> addrToSessionId;
};

#endif // !SESSION_MANAGER_H