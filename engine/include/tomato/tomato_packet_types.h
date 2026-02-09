#ifndef TOMATO_TOMATO_PACKET_TYPES_H
#define TOMATO_TOMATO_PACKET_TYPES_H
#include <typeinfo>

namespace tomato
{

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