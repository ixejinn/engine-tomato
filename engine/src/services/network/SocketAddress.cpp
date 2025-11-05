#include "tomato/services/network/SocketAddress.h"
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

namespace tomato
{
	std::string tomato::SocketAddress::ToString() const
	{
		// NOTE: Currently supports only IPv4.
		//       For IPv6, format should be [address]:port (to avoid ':' ambiguity). Planned for future extension.

		
		const sockaddr_in* s = GetAsSockAddrIn();
		char destinationBuffer[128];
		InetNtop(s->sin_family, const_cast<in_addr*>(&s->sin_addr), destinationBuffer, sizeof(destinationBuffer));
		
		return std::string(destinationBuffer) + ":" + std::to_string(ntohs(s->sin_port));
	}


}