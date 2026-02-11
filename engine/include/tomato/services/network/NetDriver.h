#ifndef TOMATO_NETDRIVER_H
#define TOMATO_NETDRIVER_H

#include "tomato/services/network/Socket.h"
#include "tomato/services/network/SocketAddress.h"
#include "tomato/services/network/CoreNetwork.h"

namespace tomato
{
	/**
	* Handles OS-level socket creation, initialization, and shutdown.
	* Performs UDP recv/send and passes received packets to the upper layer (NetworkService).
	* All external network I/O, including communication with the match server, is handled here.
	* 
	* This class does not manage threads and only performs network I/O.
	* It does not access game logic or NetConnection directly.
	* 
	* Implemented for local environment.
	*/
	class NetDriver
	{
	public:
		NetDriver();
		~NetDriver();

		bool InitSocket();
		bool SendPacket(const void* buffer, int& byteSentCount, const SocketAddress& inToAddress);
		// Returns true if data was received, false otherwise.
		bool RecvPacket(void* buffer, int& receivedBytes, SocketAddress& outFromAddress);

	private:
		SocketPtr socket_;
	};

}

#endif // !TOMATO_NETDRIVER_H