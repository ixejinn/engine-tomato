#ifndef TOMATO_SOCKET_ADDRESS_H
#define TOMATO_SOCKET_ADDRESS_H

#include <WinSock2.h>
#include <memory>
#include <string>

namespace tomato
{
	class SocketAddress
	{
	public:
		SocketAddress(uint32_t inAddress, uint16_t inPort)
		{
			GetAsSockAddrIn()->sin_family = AF_INET;
			GetIPv4Ref() = htonl(inAddress);
			GetAsSockAddrIn()->sin_port = htons(inPort);
		}

		SocketAddress(const sockaddr& inSockAddr)
		{
			memcpy(&sockAddr_, &inSockAddr, sizeof(sockaddr));
		}
		
		SocketAddress()
		{
			GetAsSockAddrIn()->sin_family = AF_INET;
			GetIPv4Ref() = INADDR_ANY;
			GetAsSockAddrIn()->sin_port = 0; //automatically allocated
		}

		bool operator==(const SocketAddress& other) const
		{
			return (sockAddr_.sa_family == AF_INET &&
					GetAsSockAddrIn()->sin_port == other.GetAsSockAddrIn()->sin_port) &&
					(GetIPv4Ref() == other.GetIPv4Ref());
		}

		uint32_t			GetSize()			const { return sizeof(sockaddr); }
		std::string			ToString()			const;

	private:
		friend class Socket;
		
		sockaddr sockAddr_;
		uint32_t&			GetIPv4Ref()				{ return *reinterpret_cast<uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr); }
		const uint32_t&		GetIPv4Ref()		const	{ return *reinterpret_cast<const uint32_t*>(&GetAsSockAddrIn()->sin_addr.S_un.S_addr); }

		sockaddr_in*		GetAsSockAddrIn()			{ return reinterpret_cast<sockaddr_in*>(&sockAddr_); }
		const sockaddr_in*	GetAsSockAddrIn()	const	{ return reinterpret_cast<const sockaddr_in*>(&sockAddr_); }
		
	}
};

#endif // !TOMATO_SOCKET_ADDRESS_H