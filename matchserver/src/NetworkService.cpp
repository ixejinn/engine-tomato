#include "NetworkService.h"
#include "PacketTypes.h"

#include <iostream>
#include <vector>
#include <limits>
#include <thread>

void NetworkService::AddNetMassage(PacketPtr& packets)
{
	TCPRecvQueue.Emplace(std::move(packets));
}

void NetworkService::Update()
{
	ProcessQueuedPackets();
	ProcessNetSendRequest();
	ProcessSendPacket();
}

void NetworkService::ProcessSendPacket()
{
	std::vector<tomato::TCPSocketPtr> writableSockets;
	sessionMgr_.GetWritableSockets(writableSockets);

	if (writableSockets.empty())
		return;

	for (const tomato::TCPSocketPtr& socket : writableSockets)
	{
		if (!sessionMgr_.ValidateSession(socket))
			continue;

		TCP::Session* session = sessionMgr_.GetSession(socket);
		if (session->sendBuffer.empty())
			continue;

		int sent = socket->Send(session->sendBuffer.data(), session->sendBuffer.size());
		if (sent > 0)
		{
			std::cout << __FUNCTION__ << '\n';
			//
			//tomato::NetBitReader rd{ session->sendBuffer.data(), static_cast<int16_t>(session->sendBuffer.size()) };
			//uint16_t readSize{ 0 }, readType{ 0 };

			//rd.ReadInt(readSize, std::numeric_limits<uint16_t>::max());
			//rd.ReadInt(readType, static_cast<uint16_t>(TCPPacketType::COUNT));

			//std::cout << int(readSize) << "type : " << int(readType) << '\n';

			session->ConsumeSendBuffer(sent);
		}
	}
}

void NetworkService::ProcessNetSendRequest()
{
	while (!NetSendRequestQueue.Empty())
	{
		SendCommandPtr sendCommand;
		NetSendRequestQueue.Dequeue(sendCommand);

		sessionMgr_.AppendSendBuffer(sendCommand->sessionId, sendCommand->data.data(), sendCommand->size);

		//tomato::NetBitReader rd{ sendCommand->data.data(), static_cast<int16_t>(sendCommand->size) };
		//uint16_t readSize{ 0 }, readType{ 0 };
		//rd.ReadInt(readSize, std::numeric_limits<uint16_t>::max());
		//rd.ReadInt(readType, static_cast<uint16_t>(TCPPacketType::COUNT));
		std::cout << __FUNCTION__ << '\n';
		//std::cout << sendCommand->sessionId << " " << int(readSize) << " " << int(readType) << '\n';
	}
}

void NetworkService::ProcessPacket(const TCPPacketType& header, tomato::NetBitReader& reader, SessionId& client)
{
	std::cout << __FUNCTION__;
	switch (header)
	{
	case TCPPacketType::MATCH_REQUEST:
	case TCPPacketType::MATCH_CANCEL:
	case TCPPacketType::MATCH_INTRO_SUCCESS:
	case TCPPacketType::MATCH_INTRO_FAILED:
		std::cout << "::MATCH_PACKET\n";
		HandlePacketRequest(header, reader, client);
		break;

	case TCPPacketType::TIME_SYNC_REQ:
		std::cout << "::TIME_SYNC_REQ\n";
		HandlePacketTimeSync(header, reader, client);
		break;
	}
}

void NetworkService::HandlePacketRequest(const TCPPacketType& header, tomato::NetBitReader& reader, SessionId& client)
{
	uint16_t nameSize{}, port{};
	std::string name{};
	uint8_t tmp;
	MatchId matchId{0};

	tomato::SocketAddress addr = sessionMgr_.GetAddress(client);

	switch (header)
	{
	case TCPPacketType::MATCH_REQUEST:
	{
		// name size + name + udp port number
		reader.ReadInt(nameSize, std::numeric_limits<uint16_t>::max());
		for(int i = 0; i < nameSize; i++)
		{
			reader.ReadInt(tmp, std::numeric_limits<uint8_t>::max());
			name += tmp;
		}
		reader.ReadInt(port, std::numeric_limits<uint16_t>::max());

		sessionMgr_.SetSessionPort(client, port);

		MatchRequestQueue.Emplace(client, sessionMgr_.GetAddress(client), name, matchId, MatchRequestAction::Enqueue);
		break;
	}

	case TCPPacketType::MATCH_INTRO_SUCCESS:
	{
		reader.ReadInt(matchId, std::numeric_limits<MatchId>::max());
		MatchRequestQueue.Emplace(client, addr, name, matchId, MatchRequestAction::Success);
		break;
	}
	case TCPPacketType::MATCH_CANCEL:
	case TCPPacketType::MATCH_INTRO_FAILED:
	{
		reader.ReadInt(matchId, std::numeric_limits<MatchId>::max());
		MatchRequestQueue.Emplace(client, addr, name, matchId, MatchRequestAction::Cancel);
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

		std::cout << "[send] : " << client << " " << int(TCPPacketType::TIME_SYNC_RES) << " " << serverSteadyNow << '\n';
		break;
	}
	}
}

void NetworkService::ProcessQueuedPackets()
{
	//패킷 헤더 해석 및 처리 / 패킷 헤더에 따라 패킷 처리 후 pop
	while (!TCPRecvQueue.Empty())
	{
		PacketPtr nextPacket;
		TCPRecvQueue.Dequeue(nextPacket);

		uint16_t type;
		tomato::NetBitReader reader(nextPacket->buffer.data(), nextPacket->size());
		reader.ReadInt(type, static_cast<uint16_t>(TCPPacketType::COUNT));

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

	uint8_t segment[MAX_PACKET_SIZE]{};
	while (isThreadRunning)
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

					std::cout << "[New Connect] " << newClientAddress.ToString() <<
						" | session num." << sessionMgr_.GetSessionId(newSocket) << '\n';
				}
			}
			else
			{
				SessionId id = sessionMgr_.GetSessionId(socket);
				int dataReceived = socket->Receive(segment, MAX_PACKET_SIZE);

				if (dataReceived > 0)
				{
					std::cout << "GetMessage from " << id << '\n';
					ProcessDataFromClient(id, segment, dataReceived);
				}
				else if (dataReceived <= 0)
				{
					std::cout << "[Disconnected] " << sessionMgr_.GetAddress(id).ToString() << " | session num." << id << '\n';

					readBlockSockets.erase(
						std::remove(readBlockSockets.begin(), readBlockSockets.end(), socket),
						readBlockSockets.end());

					sessionMgr_.RemoveSession(id);
				}
			}
			readableSockets = readBlockSockets;
		}
	}
}

void NetworkService::ThreadStart()
{
	isThreadRunning = true;
	recvThread = std::thread(&NetworkService::TCPRecvThreadLoop, this);
}

void NetworkService::ThreadStop()
{
	isThreadRunning = false;
	recvThread.join();
}