#include "Session.h"
#include "PacketTypes.h"

namespace TCP
{
	void Session::AppendRecvBuffer(const uint8_t* data, int len)
	{
		recvBuffer.insert(recvBuffer.end(), data, data + len);
	}

	void Session::AppendSendBuffer(const uint8_t* data, int len)
	{
		sendBuffer.insert(sendBuffer.end(), data, data + len);
	}

	bool Session::ParsePacket(std::vector<uint8_t>& outData)
	{
		if (recvBuffer.size() >= sizeof(TCPHeader))
		{
			TCPHeader* header = reinterpret_cast<TCPHeader*>(recvBuffer.data());
			if (recvBuffer.size() < header->size)
				return false;

			outData.resize(header->size);
			std::memcpy(outData.data(), recvBuffer.data(), header->size);
			
			recvBuffer.erase(recvBuffer.begin(), recvBuffer.begin() + header->size);

			return true;
		}
		
		return false;
	}

	void Session::ConsumeSendBuffer(int len)
	{
		sendBuffer.erase(sendBuffer.begin(), sendBuffer.begin() + len);
	}
}