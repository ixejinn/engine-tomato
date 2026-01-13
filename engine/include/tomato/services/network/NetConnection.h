#ifndef TOMATO_NETCONNECTION_H
#define TOMATO_NETCONNECTION_H

#include <typeinfo>
#include <string>

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

	struct NetConnection
	{
		uint32_t playerId;
		std::string name;
		SocketAddress addr;
	};
}

#endif // !TOMATO_NETCONNECTION_H