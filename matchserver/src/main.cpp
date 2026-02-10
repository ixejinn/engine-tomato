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
	//MatchServer server;
	//server.Run();

	tomato::SPSCQueue<MatchRequestCommand, 128> MatchRequestQueue;
	tomato::SPSCQueue<SendCommandPtr, 256> NetSendRequest;
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

#elif 0
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
	
#elif 0
	ServerTimeMs serverSteadyNow = static_cast<ServerTimeMs>(
		duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now().time_since_epoch()).count());

	RawBuffer rb{};
	tomato::NetBitWriter wr{ &rb };
	wr.WriteInt(static_cast<uint16_t>(0), std::numeric_limits<uint16_t>::max());
	wr.WriteInt(static_cast<uint16_t>(TCPPacketType::TIME_SYNC_RES), static_cast<uint16_t>(TCPPacketType::COUNT));
	wr.WriteInt(static_cast<uint32_t>(serverSteadyNow), std::numeric_limits<ServerTimeMs>::max());

	uint16_t size = wr.GetByteSize();
	std::memcpy(rb.data(), &size, sizeof(uint16_t));

	tomato::NetBitReader rd{ rb.data(), static_cast<int16_t>(size)};
	uint16_t readSize{ 0 }, readType{ 0 };
	ServerTimeMs readTime{ 0 };

	rd.ReadInt(readSize, std::numeric_limits<uint16_t>::max());
	rd.ReadInt(readType, static_cast<uint16_t>(TCPPacketType::COUNT));
	rd.ReadInt(readTime, std::numeric_limits<ServerTimeMs>::max());

	for (int i = 0; i < size; i++)
		cout << int(rb.data()[i]) << " ";
	cout << '\n';

	cout << int(readSize) << ", " << int(readType) << ", " << int(serverSteadyNow) << " : " << int(readTime);

	//cout << int(rb->data()[0]);

#elif 1
	tomato::TCPSocketPtr sockets[4];

	RawBuffer rawBuffer;
	tomato::NetBitWriter helloPacket{ &rawBuffer };
	helloPacket.WriteInt(static_cast<uint16_t>(0), std::numeric_limits<uint16_t>::max());
	helloPacket.WriteInt(static_cast<uint16_t>(TCPPacketType::MATCH_REQUEST), static_cast<uint16_t>(TCPPacketType::COUNT));
	uint16_t pSize = helloPacket.GetByteSize();
	std::memcpy(rawBuffer.data(), &pSize, sizeof(uint16_t));

	for (int i = 0; i < 4; i++)
	{
		sockets[i] = tomato::TCPSocket::CreateTCPSocket();
		sm.GenerateSession(sockets[i], addr[i]);
		auto packet = sm.AppendRecvBuffer(sm.GetSessionId(sockets[i]), rawBuffer.data(), pSize);
		if (packet == nullptr)
		{
			cout << "nullptr packet\n";
			continue;
		}

		ns.AddNetMassage(packet);
	}

	while (true)
	{
		PushPacket(sm, ns);
		ns.Update(0.016f);
		mm.Update(0.016f);
	}

#endif // 0

	
	return 0;
}