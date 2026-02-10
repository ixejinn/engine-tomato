#ifndef TOMATO_TOMATO_PACKET_TYPES_H
#define TOMATO_TOMATO_PACKET_TYPES_H
#include <typeinfo>

using MatchId = uint16_t;
using RequestId = uint32_t;
using SessionId = uint32_t;
using ServerTimeMs = uint32_t;

namespace tomato
{
	struct Packet
	{
		RawBuffer* buffer{ nullptr };
		std::size_t size;
		SocketAddress addr;

		Packet() = default;
		Packet(RawBuffer* bufPtr, std::size_t size, SocketAddress addr)
			: buffer(bufPtr), size(size), addr(addr) {
		}
	};

	struct TCPPacket
	{
		std::vector<uint8_t> buffer;
		SessionId sessionId;

		explicit TCPPacket(std::size_t size, SessionId client)
			: buffer(size), sessionId(client) {
		}

		//Not allow to copy, only move
		TCPPacket(const TCPPacket&) = delete;
		TCPPacket& operator= (const TCPPacket&) = delete;

		TCPPacket(TCPPacket&&) = default;
		TCPPacket& operator=(TCPPacket&&) = default;

		size_t size() const { return buffer.size(); }
	};

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
		uint16_t size; // Total packet size (include header)
		TCPPacketType type; // Packet type (ex: 1 = MatchRequest, 2 = MatchCancel...)
	};
	#pragma pack(pop)
}

#endif // !TOMATO_TOMATO_PACKET_TYPES_H