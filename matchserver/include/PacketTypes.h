#ifndef PACKET_TYPES_H
#define PACKET_TYPES_H

#include <typeinfo>
#include <tomato/services/network/SocketAddress.h>

struct Packet
{
	RawBuffer* buffer{ nullptr };
	std::size_t size;
	tomato::SocketAddress addr;

	Packet() = default;
	Packet(RawBuffer* bufPtr, std::size_t size, tomato::SocketAddress addr)
		: buffer(bufPtr), size(size), addr(addr) {
	}
};

enum class PacketHeader : uint8_t
{
	NONE,
	CS_HELLO = 4,
	CS_WELCOME,
	MATCH_REQUEST,
	MATCH_INTRO,
	INTRO_RESULT,
	MATCH_START,

	COUNT
};

enum class P2PHeader : uint8_t
{
	P2P_HELLO = 4,
	P2P_WELCOME,

	COUNT
};
#endif // !PACKET_TYPES_H