#ifndef SESSION_H
#define SESSION_H
#include <vector>
#include <tomato/services/network/SocketAddress.h>
#include <tomato/services/network/TCPNetDriver.h>
#include "ServerTypes.h"

struct Packet;
namespace TCP
{
	class Session
	{
	public:
		//Session(tomato::SocketAddress& addr) : driver(tomato::TCPNetDriver(addr)) {};
		Session(tomato::TCPSocketPtr socket, tomato::SocketAddress& addr) : socket(socket), addr(addr) {};
		
		void AppendRecvBuffer(const uint8_t* data, int len);
		void AppendSendBuffer(const uint8_t* data, int len);
		bool ParsePacket(std::vector<uint8_t>& outData);
		void ConsumeSendBuffer(int len);

		const tomato::TCPSocketPtr GetSocket() const { return socket; }
		const tomato::SocketAddress& GetSocketAddress() const { return addr; }
		std::vector<uint8_t>& GetRecvBuffer() { return recvBuffer; }
		std::vector<uint8_t>& GetSendBuffer() { return sendBuffer; }

	//private:
		//tomato::TCPNetDriver driver;

		tomato::TCPSocketPtr socket;
		tomato::SocketAddress addr;
		std::vector<uint8_t> recvBuffer;
		std::vector<uint8_t> sendBuffer;
	};
}

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