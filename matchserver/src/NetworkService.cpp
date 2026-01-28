#include "NetworkService.h"
#include "PacketTypes.h"

#include <iostream>

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

