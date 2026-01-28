#include "MatchServer.h"
#include "MatchManager.h"
#include "SessionManager.h"
#include "NetworkService.h"
#include "PacketTypes.h"

#include <tomato/containers/MemoryPool.h>
#include <tomato/net/NetBitWriter.h>
#include <tomato/net/NetBitReader.h>

#include <iostream>
int main()
{
	//MatchServer server;
	//server.Run();

	tomato::SPSCQueue<MatchRequestCommand, 128> MatchRequestQueue;
	tomato::SPSCQueue<uint32_t, 256> NetSendRequest;
	MatchManager mm(MatchRequestQueue, NetSendRequest);
	SessionManager sm;
	NetworkService ns(sm, mm, MatchRequestQueue, NetSendRequest);

	//RecvPacket///////////////////////////////////////////////
	RawBuffer rawBuffer;
	tomato::NetBitWriter helloPacket{ &rawBuffer };

	helloPacket.WriteInt(static_cast<uint8_t>(PacketHeader::MATCH_REQUEST), 8);
	tomato::NetBitReader reader(rawBuffer.data(), rawBuffer.size());

	uint8_t header = -1;
	reader.ReadInt(header, 8);
	std::cout << int(header);
	std::cout << '\n' << int(PacketHeader::MATCH_REQUEST) << '\n';

	tomato::SocketAddress addr[4];
	addr[0] = { "192.168.55.165", 7777 };
	addr[1] = { "192.168.55.166", 7777 };
	addr[2] = { "192.168.55.167", 7777 };
	addr[3] = { "192.168.55.168", 7777 };
	//////////////////////////////////////////////////////////

	for (auto& a : addr)
	{
		if (!sm.ValidateSession(a))
		{
			std::cout << "Issue Session\n";
			sm.GenerateSession(a);
		}

		if (header == static_cast<uint8_t>(PacketHeader::MATCH_REQUEST))
		{
			std::cout << "Push MatchQueue\n";
			mm.AddMatchRequestQueue(sm.GetSessionId(a), sm.GetSessionId(a));
		}
	}

	while (true)
	{
		mm.Update(0.016f);
		sm.Update();
		ns.Update(0.016f);
	}
	
	return 0;
}