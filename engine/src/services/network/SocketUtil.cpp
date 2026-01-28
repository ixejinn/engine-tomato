#include "tomato/services/network/SocketUtil.h"
#include "tomato/Logger.h"

namespace tomato
{
	bool SocketUtil::InitWinsock()
	{
		WSADATA wsaData;
		int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != NO_ERROR)
		{
			TMT_ERR << "Failed to SocketUtil::InitWinsock";
			return false;
		}
		return true;
	}

	void SocketUtil::CleanUp()
	{
		WSACleanup();
	}
}