#include "Match.h"
#include <iostream>

Match::Match(const MatchContext& ctx) : ctx_(ctx), timer_(0.f) {}

MatchUpdateResult Match::Update(float dt, tomato::SPSCQueue<SendRequestCommand, 256>& sendRequestQ)
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
		std::cout << "InfoSent" << '\n';
		ctx_.state = RequestToSendNetConnection(sendRequestQ, ctx_.players);
		return MatchUpdateResult::None;
	}

	case MatchState::WaitPeerReady:
	{
	// 모든 피어들의 피어간 연결 성공 패킷이 다 도착할 때까지 대기
	// 일정 시간이 지나면 timeout으로 Failed 처리
		std::cout << "WaitPeerReady" << '\n';
		if (peerAck.all())
		{

		}
		if (timer_ >= MatchConstants::CONNECT_TIMEOUT_SEC)
		{
			ctx_.state = MatchState::Failed;
			return MatchUpdateResult::Failed;
		}
		else
		{
			ctx_.state = MatchState::AllReady;
			return MatchUpdateResult::ReadyToStart;
		}
	}

	case MatchState::Failed:
		std::cout << "Failed" << '\n';
		ctx_.state = MatchState::Processing;
		break;

	case MatchState::AllReady:
		std::cout << "Done" << '\n';
		ctx_.state = MatchState::Processing;
		break;
	}

	return MatchUpdateResult::None;
}

MatchState Match::CollectNetConnection(const MatchRequest* matchRequest)
{
	for (int i = 0; i < MatchConstants::MAX_MATCH_PLAYER; i++)
	{
		(conn + i)->matchId = ctx_.matchId;
		(conn + i)->playerId = i;
		(conn + i)->name = "name";

		int len{};
		tomato::SocketAddress address;
		if ((matchRequest + i)->socket->GetSocketAddress(address, len) == 0)
			(conn + i)->addr = address;
		else
			return MatchState::Failed;
	}
	return MatchState::InfoSent;
}

MatchState Match::RequestToSendNetConnection(tomato::SPSCQueue<SendRequestCommand, 256>& sendRequestQ, const MatchRequest* matchRequest)
{
	TCPHeader header{ sizeof(TCPHeader), TCPPacketType::MATCH_INTRO };
	uint8_t matchStartInfo{};
	for (int i = 0; i < MatchConstants::MAX_MATCH_PLAYER; i++)
	{
		SendRequestCommand sendReqCmd
		{
			.socket = (matchRequest + i)->socket,
			.header = header,
			.data = matchStartInfo
		};
		
		sendRequestQ.Emplace(sendReqCmd);
	}

	return MatchState::WaitPeerReady;
}

MatchState Match::ProcessIntroResult(tomato::SPSCQueue<MatchRequestCommand, 128>& MatchRequestQ)
{
	
	return MatchState();
}

int Match::GetPlayerId(tomato::TCPSocketPtr client)
{
	int len{};
	tomato::SocketAddress addr{};
	client->GetSocketAddress(addr, len);
	for (int i = 0; i < MatchConstants::MAX_MATCH_PLAYER; i++)
	{
		if (addr == conn[i].addr)
			return i;
	}

	return -1;
}

void Match::SetPeerAck(int idx, int set)
{
	peerAck.set(idx, set);
}