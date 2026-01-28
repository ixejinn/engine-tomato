#ifndef TOMATO_WINSOCK_CONTEXT
#define TOMATO_WINSOCK_CONTEXT
#include "tomato/services/network/SocketUtil.h"

namespace tomato
{
	/**
	* Creation order requirement:
	* 1. WinsockContext
	* 2. NetDriver
	*	
	* NetDriver creates and initializes sockets internally.
	* WSAStartup() is called inside WinsockContext,
	* so it must exist before NetDriver.
	*/

	class WinsockContext
	{
	public:
		WinsockContext() { SocketUtil::InitWinsock(); }
		~WinsockContext() { SocketUtil::CleanUp(); }
	};
}

#endif // !TOMATO_WINSOCK_CONTEXT