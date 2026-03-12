#include "MatchServer.h"
#include "MatchManager.h"
#include "SessionManager.h"
#include "NetworkService.h"
#include "PacketTypes.h"

#include <tomato/containers/MemoryPool.h>
#include <tomato/net/NetBitWriter.h>
#include <tomato/net/NetBitReader.h>

#include <iostream>
#include <limits>
#include <conio.h>
#include <thread>
#include <chrono>

using namespace std;

void PushPacket(SessionManager& sm, NetworkService& ns)
{
	if (_kbhit())
	{
		char c = _getch();

		RawBuffer rawBuffer{};
		tomato::NetBitWriter helloPacket{ &rawBuffer };

		switch (c)
		{
		case '1': //intro success
		{
			helloPacket.WriteInt(static_cast<uint16_t>(0), std::numeric_limits<uint16_t>::max());
			helloPacket.WriteInt(static_cast<uint16_t>(TCPPacketType::MATCH_INTRO_SUCCESS), static_cast<uint16_t>(TCPPacketType::COUNT));
			helloPacket.WriteInt(static_cast<uint16_t>(1), std::numeric_limits<uint16_t>::max());
			uint16_t pSize = helloPacket.GetByteSize();
			std::memcpy(rawBuffer.data(), &pSize, sizeof(uint16_t));

			for (int i = 1; i <= 4; i++)
			{
				auto packet = sm.AppendRecvBuffer(i, rawBuffer.data(), pSize);
				ns.AddNetMassage(packet);
			}
			break;
		}

		case '2': //intro fail
		{
			helloPacket.WriteInt(static_cast<uint16_t>(0), std::numeric_limits<uint16_t>::max());
			helloPacket.WriteInt(static_cast<uint16_t>(TCPPacketType::MATCH_INTRO_FAILED), static_cast<uint16_t>(TCPPacketType::COUNT));
			helloPacket.WriteInt(static_cast<uint16_t>(1), std::numeric_limits<uint16_t>::max());
			uint16_t pSize = helloPacket.GetByteSize();
			std::memcpy(rawBuffer.data(), &pSize, sizeof(uint16_t));

			for (int i = 1; i <= 4; i++)
			{
				auto packet = sm.AppendRecvBuffer(i, rawBuffer.data(), pSize);
				ns.AddNetMassage(packet);
			}
			break;
		}

		case '3': //time sync req
		{
			helloPacket.WriteInt(static_cast<uint16_t>(0), std::numeric_limits<uint16_t>::max());
			helloPacket.WriteInt(static_cast<uint16_t>(TCPPacketType::TIME_SYNC_REQ), static_cast<uint16_t>(TCPPacketType::COUNT));
			uint16_t pSize = helloPacket.GetByteSize();
			std::memcpy(rawBuffer.data(), &pSize, sizeof(uint16_t));

			for (int i = 1; i <= 4; i++)
			{
				auto packet = sm.AppendRecvBuffer(i, rawBuffer.data(), pSize);
				ns.AddNetMassage(packet);
			}
			break;
		}

		}
	}
}
int main()
{
	MatchServer server;
	server.Run();

	return 0;
}