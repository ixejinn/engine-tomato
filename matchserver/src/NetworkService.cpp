#include "NetworkService.h"
#include "PacketTypes.h"

#include <iostream>
#include <vector>

void NetworkService::Update(float dt)
{
	ProcessPendingPacket();
	ProcessNetSendRequest();
}

void NetworkService::SendPacket(uint8_t messageType)
{
	
	//SendPacket
	//driver_.SendPacket(messageType);
}

void NetworkService::ProcessPendingPacket()
{
	//seperate later
	Packet packet;
	PacketHeader messageType = PacketHeader::NONE;
	SessionId sessionId = -1;

	while (!NetRecvQueue.Empty())
	{
		NetRecvQueue.Dequeue(packet);
		tomato::NetBitReader reader(packet.buffer->data(), packet.size);

		//Check Session
		//@TODO : Add session discrimination logic for first and subsequent packets
		//		  Assume there is no session information now
		if (sessionMgr_.ValidateSession(packet.addr))
		{
			sessionId = sessionMgr_.GetSessionId(packet.addr);
			sessionMgr_.UpdateLastRecv(sessionId);
		}
		else
			sessionMgr_.GenerateSession(packet.addr);

		uint16_t header = -1;
		reader.ReadInt(header, static_cast<uint32_t>(PacketHeader::COUNT)); //Modify after adding a function for enum
		messageType = static_cast<PacketHeader>(header); //Modify after adding a function for enum
		
		switch (messageType)
		{
		case PacketHeader::MATCH_REQUEST:
			ProcessPacketMatchReq(reader, sessionId);
			break;
		}
	}
}

void NetworkService::ProcessPacketMatchReq(tomato::NetBitReader& reader, SessionId sessionId)
{
	RequestId reqId = -1;
	uint8_t tAction = -1;
	MatchRequestAction action = MatchRequestAction::NONE;

	reader.ReadInt(reqId, static_cast<uint32_t>(reqId));
	reader.ReadInt(tAction, static_cast<uint32_t>(tAction));
	action = static_cast<MatchRequestAction>(tAction);

	MatchRequestCommand reqCommand{ sessionId, reqId, action };
	MatchRequestQueue.Emplace(reqCommand);
}


void NetworkService::ProcessNetSendRequest()
{
	while(!NetSendRequestQueue.Empty())
	{
		uint32_t packet{};
		NetSendRequestQueue.Dequeue(packet);

		if (packet)
		{
			PacketHeader header = static_cast<PacketHeader>(packet);
			std::cout << "ProcessNetSendRequest::";
			switch (header)
			{
			case PacketHeader::CS_WELCOME:
				std::cout << "CS_WELCOME\n";
				break;

			case PacketHeader::MATCH_INTRO:
				std::cout << "MATCH_INTRO\n";
				break;

			case PacketHeader::MATCH_START:
				std::cout << "MATCH_START\n";
				break;
			}
		}
	}
}
void NetworkService::ProcessPacket(const TCPHeader& header, tomato::NetBitReader& reader, tomato::TCPSocketPtr& client)
{
	switch (header.type)
	{
	case TCPPacketType::MATCH_REQUEST:
		ProcessPacketRequest(reader, client);
		break;
	}
}

void NetworkService::ProcessPacketRequest(tomato::NetBitReader& reader, tomato::TCPSocketPtr& client)
{
	//패킷 읽어서 해석 x 그냥 리퀘스트 커맨드 구조체로 큐에 넣어줌
	uint8_t tAction = -1;
	MatchRequestAction action = MatchRequestAction::NONE;

	reader.ReadInt(tAction, static_cast<uint32_t>(tAction));
	action = static_cast<MatchRequestAction>(tAction);

	MatchRequestCommand reqCommand{ 0, 0, action }; // 0, 0 -> client
	MatchRequestQueue.Emplace(reqCommand);
}

void NetworkService::ProcessQueuedPackets()
{
	//패킷 헤더에 따라 패킷 처리 후 pop
	while (!TCPRecvQueue.Empty())
	{
		TCPPacket nextPacket;
		TCPRecvQueue.Dequeue(nextPacket);

		TCPHeader header;
		uint16_t type;
		tomato::NetBitReader reader(nextPacket.buffer.data(), nextPacket.size());
		reader.ReadInt(header.size, static_cast<uint32_t>(sizeof(TCPHeader::size)));
		reader.ReadInt(type, static_cast<uint32_t>(sizeof(TCPHeader::type)));
		header.type = static_cast<TCPPacketType>(type);

		ProcessPacket(header, reader, nextPacket.client);
	}
}

void NetworkService::ProcessDataFromClient(const tomato::TCPSocketPtr client, const uint8_t* data, const int len)
{
	//socket에 따라 세션에 지정된 vector에 데이터스트림 추가
	if (!sessionMgr_.ValidateSession(client))
		return;
	
	//client buffer에 데이터스트림 누적 후, 추출할 사이즈만큼 쌓이면 추출
	std::vector<uint8_t> buf;
	if (sessionMgr_.AppendRecvBuffer(client, data, len, buf))
	{
		//TCPPacket packet{ buf.data(), len, client};
		TCPRecvQueue.Emplace(buf.data(), len, client);
	}
}

void NetworkService::TCPRecvThreadLoop()
{
	tomato::TCPSocketPtr listenSocket = tcpDriver_.GetSocket();
	std::vector<tomato::TCPSocketPtr> readBlockSockets;
	readBlockSockets.push_back(listenSocket);
	std::vector<tomato::TCPSocketPtr> readableSockets;

	bool tmpRun = true;
	while (tmpRun)
	{
		if (!tomato::SocketUtil::Select(&readBlockSockets, &readableSockets,
			nullptr, nullptr, nullptr, nullptr))
			continue;

		for (const tomato::TCPSocketPtr& socket : readableSockets)
		{
			if (socket == listenSocket)
			{
				tomato::SocketAddress newClientAddress;
				auto newSocket = listenSocket->Accept(newClientAddress);
				if (newSocket)
				{
					readBlockSockets.push_back(newSocket);
					sessionMgr_.GenerateSession(newSocket, newClientAddress);
				}
			}
			else
			{
				uint8_t segment[MAX_PACKET_SIZE];
				int dataReceived = socket->Receive(segment, MAX_PACKET_SIZE);

				if (dataReceived > 0)
					ProcessDataFromClient(socket, segment, dataReceived);
			}
		}
	}
}