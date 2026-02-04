#ifndef PACKET_TYPES_H
#define PACKET_TYPES_H

#include <typeinfo>
#include <tomato/services/network/SocketAddress.h>
#include <tomato/services/network/TCPSocket.h>

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

struct TCPPacket
{
	std::vector<uint8_t> buffer;
	tomato::TCPSocketPtr client;
	
	TCPPacket() = default;
	TCPPacket(const uint8_t* data, std::size_t size, tomato::TCPSocketPtr client)
		: buffer(data, data + size), client(client) {
	}
	size_t size() const { return buffer.size(); }
};

enum class TCPPacketType : uint16_t
{
	MATCH_REQUEST,
	MATCH_CANCEL,
	MATCH_INTRO,
	TIME_SYNC_REQ,
	TIME_SYNC_RES,
	READY_ACK,
	MATCH_START,

	COUNT
};

#pragma pack(push, 1)
struct TCPHeader
{
	uint16_t size; // Total packet length (following data length)
	TCPPacketType type; // Packet type (ex: 1 = MatchRequest, 2 = MatchCancel...)
};
#pragma pack(pop)


///////////////////////////////////////
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