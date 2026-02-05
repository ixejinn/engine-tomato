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
	SessionId sessionId;

	explicit TCPPacket(std::size_t size, SessionId client)
		: buffer(size), sessionId(client) {}

	//Not allow to copy, only move
	TCPPacket(const TCPPacket&) = delete;
	TCPPacket& operator= (const TCPPacket&) = delete;

	TCPPacket(TCPPacket&&) = default;
	TCPPacket& operator=(TCPPacket&&) = default;

	size_t size() const { return buffer.size(); }
};

using PacketPtr = std::unique_ptr<TCPPacket>;

enum class TCPPacketType : uint16_t
{
	MATCH_REQUEST,
	MATCH_CANCEL,
	MATCH_INTRO,
	TIME_SYNC_REQ,
	TIME_SYNC_RES,
	MATCH_INTRO_SUCCESS,
	MATCH_INTRO_FAILED,
	READY_ACK,
	MATCH_START,

	COUNT
};

#pragma pack(push, 1)
struct TCPHeader
{
	uint16_t size; // Total packet length (following data length) byte
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