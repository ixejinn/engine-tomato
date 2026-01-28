#ifndef SESSION_H
#define SESSION_H

#include <typeinfo>
#include <tomato/services/network/SocketAddress.h>
#include "ServerTypes.h"

namespace SessionConstants
{
	constexpr ServerTimeMs SESSION_TIMEOUT_MS = 1000 * 60 * 5;
}

class Session
{
public:
	Session(SessionId id, tomato::SocketAddress addr, ServerTimeMs now) : id(id), addr(addr), lastRecvTime(now) {};

	bool IsTimeOut(ServerTimeMs now) const
	{
		return (now - lastRecvTime) > SessionConstants::SESSION_TIMEOUT_MS;
	}
	void UpdateLastRecv(ServerTimeMs now)
	{
		lastRecvTime = now;
	}

	SessionId GetSessionId () const { return id; }
	const tomato::SocketAddress& GetSessionAddr() const { return addr; }

private:
	SessionId id;
	tomato::SocketAddress addr;
	ServerTimeMs lastRecvTime;
};

#endif // !SESSION_H