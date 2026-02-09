#include "NetworkService.h"
#include "PacketTypes.h"

#include <iostream>
#include <vector>
#include <limits>

void NetworkService::AddNetMassage(PacketPtr& packets)
{
	TCPRecvQueue.Emplace(std::move(packets));
}

void NetworkService::Update(float dt)
{
	ProcessQueuedPackets();
	//ProcessSendPacket();


	//UDP
	//ProcessPendingPacket();
	//ProcessNetSendRequest();
}
//
//void NetworkService::ProcessPendingPacket()
//{
//	//seperate later
//	Packet packet;
//	PacketHeader messageType = PacketHeader::NONE;
//	SessionId sessionId = -1;
//
//	while (!NetRecvQueue.Empty())
//	{
//		NetRecvQueue.Dequeue(packet);
//		tomato::NetBitReader reader(packet.buffer->data(), packet.size);
//
//		//Check Session
//		//@TODO : Add session discrimination logic for first and subsequent packets
//		//		  Assume there is no session information now
//		if (sessionMgr_.ValidateSession(packet.addr))
//		{
//			sessionId = sessionMgr_.GetSessionId(packet.addr);
//			sessionMgr_.UpdateLastRecv(sessionId);
//		}
//		else
//			sessionMgr_.GenerateSession(packet.addr);
//
//		uint16_t header = -1;
//		reader.ReadInt(header, static_cast<uint32_t>(PacketHeader::COUNT)); //Modify after adding a function for enum
//		messageType = static_cast<PacketHeader>(header); //Modify after adding a function for enum
//		
//		switch (messageType)
//		{
//		case PacketHeader::MATCH_REQUEST:
//			ProcessPacketMatchReq(reader, sessionId);
//			break;
//		}
//	}
//}
// 
//void NetworkService::ProcessPacketMatchReq(tomato::NetBitReader& reader, SessionId sessionId)
//{
//	RequestId reqId = -1;
//	uint8_t tAction = -1;
//	MatchRequestAction action = MatchRequestAction::NONE;
//
//	reader.ReadInt(reqId, static_cast<uint32_t>(reqId));
//	reader.ReadInt(tAction, static_cast<uint32_t>(tAction));
//	action = static_cast<MatchRequestAction>(tAction);
//
//	MatchRequestCommand reqCommand{ sessionId, reqId, action };
//	MatchRequestQueue.Emplace(reqCommand);
//}

//void NetworkService::ProcessSendPacket()
//{
//	//if sendBuffer have data, Send it
//	std::vector<tomato::TCPSocketPtr> writeCandidates;
//	std::vector<tomato::TCPSocketPtr> writableSockets;
//
//	sessionMgr_.GetWritableSockets(writeCandidates);
//
//	if (writeCandidates.empty())
//		return;
//
//	if (!tomato::SocketUtil::Select(nullptr, nullptr,
//		&writeCandidates, &writableSockets, nullptr, nullptr))
//		return;
//
//	for (const tomato::TCPSocketPtr& socket : writableSockets)
//	{
//		if (!sessionMgr_.ValidateSession(socket))
//			continue;
//
//		TCP::Session* session = sessionMgr_.GetSession(socket);
//		if (session->sendBuffer.empty())
//			continue;
//
//		int sent = socket->Send(session->sendBuffer.data(), session->sendBuffer.size());
//		if(sent > 0)
//			session->ConsumeSendBuffer(sent);
//		else //disconnected or error
//			sessionMgr_.RemoveSession(socket);
//	}
//}

void NetworkService::ProcessNetSendRequest()
{
	while (!NetSendRequestQueue.Empty())
	{
		SendCommandPtr sendCommand;
		NetSendRequestQueue.Dequeue(sendCommand);

		sessionMgr_.AppendSendBuffer(sendCommand->sessionId, sendCommand->data.data(), sendCommand->size);
	}
}
void NetworkService::ProcessPacket(const TCPPacketType& header, tomato::NetBitReader& reader, SessionId& client)
{
	std::cout << __FUNCTION__ << '\n';
	switch (header)
	{
	case TCPPacketType::MATCH_REQUEST:
	case TCPPacketType::MATCH_CANCEL:
	case TCPPacketType::MATCH_INTRO_SUCCESS:
	case TCPPacketType::MATCH_INTRO_FAILED:
		std::cout << "MATCH_REQUEST_PACKET::";
		HandlePacketRequest(header, reader, client);
		break;

	case TCPPacketType::TIME_SYNC_REQ:
		HandlePacketTimeSync(header, reader, client);
		break;
	}
}

void NetworkService::HandlePacketRequest(const TCPPacketType& header, tomato::NetBitReader& reader, SessionId& client)
{
	//패킷 읽어서 해석 x 그냥 리퀘스트 커맨드 구조체로 큐에 넣어줌
	switch (header)
	{
	case TCPPacketType::MATCH_REQUEST:
		MatchRequestQueue.Emplace(client, 0, MatchRequestAction::Enqueue);
		break;

	case TCPPacketType::MATCH_INTRO_SUCCESS:

	case TCPPacketType::MATCH_CANCEL:
	case TCPPacketType::MATCH_INTRO_FAILED:
	{
		MatchId matchId;
		reader.ReadInt(matchId, std::numeric_limits<MatchId>::max());
		MatchRequestQueue.Emplace(client, matchId, MatchRequestAction::Cancel);
		break;
	}
	}
}

void NetworkService::HandlePacketTimeSync(const TCPPacketType& header, tomato::NetBitReader& reader, SessionId& client)
{
	switch (header)
	{
	case TCPPacketType::TIME_SYNC_REQ:
	{
		ServerTimeMs serverSteadyNow = static_cast<ServerTimeMs>(
			duration_cast<std::chrono::milliseconds>(
				std::chrono::steady_clock::now().time_since_epoch()).count());

		RawBuffer buf{};
		tomato::NetBitWriter writer{ &buf };
		writer.WriteInt(static_cast<uint16_t>(0), std::numeric_limits<uint16_t>::max());
		writer.WriteInt(static_cast<uint16_t>(TCPPacketType::TIME_SYNC_RES), static_cast<uint16_t>(TCPPacketType::COUNT));
		writer.WriteInt(static_cast<uint32_t>(serverSteadyNow), std::numeric_limits<ServerTimeMs>::max());

		uint16_t size = writer.GetByteSize();
		// WARNING:
		// Packet size must be the first field in the buffer.
		// Overwriting this value may behave differently depending on platform/endianness.
		memcpy(buf.data(), &size, sizeof(uint16_t));
	
		sessionMgr_.AppendSendBuffer(client, buf.data(), size);

		break;
	}
	}
}

void NetworkService::ProcessQueuedPackets()
{
	//패킷 헤더 해석 및 처리 / 패킷 헤더에 따라 패킷 처리 후 pop
	while (!TCPRecvQueue.Empty())
	{
		//std::cout << __FUNCTION__ << '\n';
		PacketPtr nextPacket;
		TCPRecvQueue.Dequeue(nextPacket);

		uint16_t type;
		tomato::NetBitReader reader(nextPacket->buffer.data(), nextPacket->size());

		reader.ReadInt(type, static_cast<uint16_t>(TCPPacketType::COUNT));
		std::cout << int(type) << '\n';

		ProcessPacket(static_cast<TCPPacketType>(type), reader, nextPacket->sessionId);
	}
}

void NetworkService::ProcessDataFromClient(const SessionId& sessionId, const uint8_t* data, const int len)
{
	//socket에 따라 세션에 지정된 vector에 데이터스트림 추가
	if (!sessionMgr_.ValidateSession(sessionId))
		return;
	
	//client buffer에 데이터스트림 누적 후, 추출할 사이즈만큼 쌓이면 추출 후 큐에 emplace
	auto packet = sessionMgr_.AppendRecvBuffer(sessionId, data, len);
	if (packet == nullptr)
		return;

	TCPRecvQueue.Emplace(std::move(packet));
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
				SessionId id = sessionMgr_.GetSessionId(socket);
				uint8_t segment[MAX_PACKET_SIZE];
				int dataReceived = socket->Receive(segment, MAX_PACKET_SIZE);

				if (dataReceived > 0)
					ProcessDataFromClient(id, segment, dataReceived);
			}
		}
	}
}