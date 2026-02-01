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
	tomato::SPSCQueue<SendRequestCommand, 256> NetSendRequest;
	MatchManager mm(MatchRequestQueue, NetSendRequest);
	SessionManager sm;
	NetworkService ns(sm, mm, MatchRequestQueue, NetSendRequest);

	//RecvPacket///////////////////////////////////////////////

	tomato::SocketAddress addr[4];
	addr[0] = { "192.168.55.165", 7777 };
	addr[1] = { "192.168.55.166", 7777 };
	addr[2] = { "192.168.55.167", 7777 };
	addr[3] = { "192.168.55.168", 7777 };
#if 0
	RawBuffer rawBuffer;
	tomato::NetBitWriter helloPacket{ &rawBuffer };

	helloPacket.WriteInt(static_cast<uint16_t>(TCPPacketType::MATCH_REQUEST), 16);
	tomato::NetBitReader reader(rawBuffer.data(), rawBuffer.size());

	uint8_t header = -1;
	reader.ReadInt(header, 8);
	std::cout << int(header);
	std::cout << '\n' << int(PacketHeader::MATCH_REQUEST) << '\n';


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

#elif 1
	tomato::TCPSocketPtr sockets[4];
	TCPPacket fakePacket[4];

	TCPHeader fakeHeader;
	fakeHeader.size = 5;
	fakeHeader.type = TCPPacketType::MATCH_REQUEST;

	RawBuffer rawBuffer;
	tomato::NetBitWriter helloPacket{ &rawBuffer };
	helloPacket.WriteInt(static_cast<uint8_t>(fakeHeader.size), 8);
	helloPacket.WriteInt(static_cast<uint8_t>(fakeHeader.type), 8);
	helloPacket.WriteInt(static_cast<uint8_t>(MatchRequestAction::Enqueue), 8);


	//std::vector<uint8_t> vec;
	//vec.push_back(static_cast<uint8_t>(fakeHeader.size));
	//vec.push_back(static_cast<uint8_t>(fakeHeader.type));
	//vec.push_back(static_cast<uint8_t>(MatchRequestAction::Enqueue));
	//std::cout << int(vec[0]) << " " << int(vec[1]) << " " << int(vec[2]) << '\n';

	//TCPHeader header;
	//uint16_t type;
	//MatchRequestAction action = MatchRequestAction::NONE;
	//uint8_t s, t, tAction = -1;
	//vec.insert(vec.begin(), rawBuffer.begin(), rawBuffer.begin() + 2);
	//tomato::NetBitReader reader(vec.data(), 2);
	//reader.ReadInt(s, 8);
	//reader.ReadInt(t, 8);
	//reader.ReadInt(tAction, 8);
	////tomato::NetBitReader reader(rawBuffer.data(), 3);
	////reader.ReadInt(header.size, static_cast<uint32_t>(sizeof(TCPHeader::size)));
	////reader.ReadInt(type, static_cast<uint32_t>(sizeof(TCPHeader::type)));
	////header.type = static_cast<TCPPacketType>(type);
	//header.size = s;
	//header.type = static_cast<TCPPacketType>(t);
	//action = static_cast<MatchRequestAction>(tAction);
	//std::cout << "SIZE: " << int(header.size) << " TYPE: " << int(header.type) << " ACTION: " << int(tAction) << '\n';

	for (int i = 0; i < 4; i++)
	{
		sockets[i] = tomato::TCPSocket::CreateTCPSocket();
		sm.GenerateSession(sockets[i], addr[i]);
		fakePacket[i].client = sockets[i];
		fakePacket[i].buffer.insert(fakePacket[i].buffer.begin(), rawBuffer.begin(), rawBuffer.end());
		//fakePacket[i].buffer.push_back(static_cast<uint8_t>(TCPPacketType::MATCH_REQUEST));
		//fakePacket[i].buffer.push_back(static_cast<uint8_t>(MatchRequestAction::Enqueue));
		//std::cout << (int)(fakePacket[i].buffer[0]) << '\n';
		ns.AddNetMassage(fakePacket[i]);
	}
	
	while (true)
	{
		ns.Update(0.016f);
		sm.Update();
		mm.Update(0.016f);
	}
	
#endif // 0

	
	return 0;
}