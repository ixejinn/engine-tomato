#ifndef TOMATO_NETCONNECTION_H
#define TOMATO_NETCONNECTION_H

#include <typeinfo>
#include <string>

#include "tomato/EngineConfig.h"
#include "tomato/tomato_packet_types.h"
#include "tomato/services/network/SocketAddress.h"

namespace tomato
{
	/**
	* Stores network information of one connected peer.
	* Packet loss and ordering are not guaranteed.
	* This struct does not perform any network I/O and is used only for storing state.
	* 
	* The following fields may be added later:
	*  - State	   : Connecting, Connected, Disconnected
	*  - Heartbeat : time when the last packet was received (lastSeenTime)
	*    If no packet is received for a certain time, the connection is considered lost.
	*/
	class NetBitWriter;
	class NetBitReader;
	struct NetConnection
	{
		uint32_t sessionId{ 0 };

		uint16_t matchId;
		PlayerId playerId;
		std::string name{};
		SocketAddress addr;
	};

	struct NetConnectionPacket
	{
		void Write(NetBitWriter& writer);
		void Read(NetBitReader& reader);

		static constexpr TCPPacketType type = TCPPacketType::MATCH_INTRO;
		NetConnection netconn[EngineConfig::MAX_PLAYER_NUM];
	};
}

#endif // !TOMATO_NETCONNECTION_H