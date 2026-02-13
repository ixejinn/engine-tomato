#include "tomato/services/network/SocketAddress.h"

#include <WS2tcpip.h>

namespace tomato
{
	bool SocketAddress::CheckMyAddress(uint32_t& inAddress)
	{
		char hostname[256];
		if (gethostname(hostname, sizeof(hostname)) == 0)
		{
			struct addrinfo* servinfo, hints;
			memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_DGRAM;
			
			if (getaddrinfo(hostname, 0, &hints, &servinfo) != 0)
				return false;

			sockaddr_in* addr_in = reinterpret_cast<sockaddr_in*>(servinfo->ai_addr);
			uint32_t& myAddr = *reinterpret_cast<uint32_t*>(&addr_in->sin_addr.S_un.S_addr);

			//char myIP[32];
			//inet_ntop(AF_INET, &(addr_in->sin_addr), myIP, sizeof(myIP));
			//printf("IP Address : %s\n", myIP);

			if (myAddr == inAddress)
			{
				freeaddrinfo(servinfo);
				return true;
			}

			freeaddrinfo(servinfo);
		}
		return false;
	}

	std::string SocketAddress::ToString() const
	{
		// NOTE: Currently supports only IPv4.
		//       For IPv6, format should be [address]:port (to avoid ':' ambiguity). Planned for future extension.

		
		const sockaddr_in* s = GetAsSockAddrIn();
		char destinationBuffer[128];
		InetNtop(s->sin_family, const_cast<in_addr*>(&s->sin_addr), destinationBuffer, sizeof(destinationBuffer));
		
		return std::string(destinationBuffer) + ":" + std::to_string(ntohs(s->sin_port));
	}


}