#include "Match.h"
#include <iostream>
#include <tomato/net/NetBitWriter.h>
#include "SessionManager.h" //for test
Match::Match(const MatchContext& ctx) : ctx_(ctx), timer_(0.f) {}

MatchUpdateResult Match::Update(float dt, tomato::SPSCQueue<SendCommandPtr, 256>& sendRequestQ)
{
	timer_ += dt;

	switch (ctx_.state)
	{
	case MatchState::Init:
	{
		std::cout << "Init" << '\n';
		ctx_.state = CollectNetConnection(ctx_.players);
		return MatchUpdateResult::None;
	}

	case MatchState::InfoSent:
	{
	// 각 피어들에게 NetConnection 정보 전송 요청
		std::cout << "[MatchState::InfoSent]" << '\n';
		ctx_.state = RequestToSendNetConnection(sendRequestQ, ctx_.players);
		return MatchUpdateResult::None;
	}

	case MatchState::WaitPeerReady:
	{
	// 모든 피어들의 피어간 연결 성공 패킷이 다 도착할 때까지 대기
	// 일정 시간이 지나면 timeout으로 Failed 처리
		//std::cout << "WaitPeerReady" << '\n';
		//std::cout << timer_ << '\n';
		if (peerAck.all())
		{
			ctx_.state = MatchState::AllReady;
			return MatchUpdateResult::ReadyToStart;
		}

		if (timer_ >= MatchConstants::CONNECT_TIMEOUT_SEC)
		{
			std::cout << "!!!!TIMEOUT";
			peerAck.reset();
			ctx_.state = MatchState::Failed;
			return MatchUpdateResult::Failed;
		}

		return MatchUpdateResult::None;
	}

	case MatchState::Failed:
		std::cout << "Failed" << '\n';
		ctx_.state = MatchState::Processing;
		return MatchUpdateResult::Failed;

	case MatchState::AllReady:
		std::cout << "Done" << '\n';
		ctx_.state = MatchState::Processing;
		return MatchUpdateResult::ReadyToStart;
	}

	return MatchUpdateResult::None;
}

MatchState Match::CollectNetConnection(const MatchRequest* matchRequest)
{
	for (int i = 0; i < MatchConstants::MAX_MATCH_PLAYER; i++)
	{
		(conn + i)->sessionId = (matchRequest + i)->sessionId;
		(conn + i)->matchId = ctx_.matchId;
		(conn + i)->playerId = i;
		(conn + i)->name = (matchRequest + i)->name;

#if 0
		int len{};
		tomato::SocketAddress address;
		//if ((matchRequest + i)->socket->GetSocketAddress(address, len) == 0)
		//	(conn + i)->addr = address;
		else
			return MatchState::Failed;
#elif 1
		tomato::SocketAddress addr[4];
		addr[0] = { "192.168.55.165", 7777 };
		addr[1] = { "192.168.55.166", 7777 };
		addr[2] = { "192.168.55.167", 7777 };
		//addr[3] = { "192.168.55.168", 7777 };
		addr[3] = { "192.168.31.234", 7777 };
		(conn + i)->addr = { "192.168.31.234", 7777 };
	}
#endif

	return MatchState::InfoSent;
}

MatchState Match::RequestToSendNetConnection(tomato::SPSCQueue<SendCommandPtr, 256>& sendRequestQ, const MatchRequest* matchRequest)
{
	RawBuffer buf{};
	tomato::NetBitWriter writer{ &buf };
	writer.WriteInt(static_cast<uint16_t>(0), std::numeric_limits<uint16_t>::max()); //packet len
	writer.WriteInt(static_cast<uint16_t>(TCPPacketType::MATCH_INTRO), static_cast<uint16_t>(TCPPacketType::COUNT));
	writer.WriteInt(static_cast<uint16_t>(ctx_.matchId), std::numeric_limits<uint16_t>::max());
	
	for (int i = 0; i < MatchConstants::MAX_MATCH_PLAYER; i++)
	{
		writer.WriteInt(static_cast<uint8_t>(conn[i].playerId), std::numeric_limits<uint8_t>::max());
		writer.WriteInt(static_cast<uint16_t>(conn[i].name.size()), std::numeric_limits<uint16_t>::max());
		for (int j = 0; j < conn[i].name.size(); j++)
			writer.WriteInt(static_cast<uint8_t>(conn[i].name[j]), std::numeric_limits<uint8_t>::max());
		writer.WriteInt(static_cast<uint32_t>(conn[i].addr.GetIPv4()), std::numeric_limits<uint32_t>::max());
	}
	uint16_t size = writer.GetByteSize();

	memcpy(buf.data(), &size, sizeof(uint16_t));

	for (int i = 0; i < MatchConstants::MAX_MATCH_PLAYER; i++)
	{
		auto sendCmd = std::make_unique<SendRequestCommand>((matchRequest + i)->sessionId, size);
		std::memcpy(sendCmd.get()->data.data(), buf.data(), size);

		sendRequestQ.Emplace(std::move(sendCmd));
	}

	return MatchState::WaitPeerReady;
}

const int Match::GetPlayerId(const SessionId& client) const
{
	for (int i = 0; i < MatchConstants::MAX_MATCH_PLAYER; i++)
	{
		if (conn[i].sessionId == client)
			return conn[i].playerId;
	}

	return -1;
}

void Match::SetPeerAck(int idx, int set)
{
	peerAck.set(idx, set);
}