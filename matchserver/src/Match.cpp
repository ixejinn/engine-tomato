#include "Match.h"
#include <iostream>

Match::Match(const MatchContext& ctx) : ctx_(ctx), timer_(0.f) {}

void Match::Update(float dt)
{
	timer_ += dt;

	switch (ctx_.state)
	{
	case MatchState::Init:
		std::cout << "Init" << '\n';
		ctx_.state = MatchState::InfoSent;
		break;

	case MatchState::InfoSent:
	// 각 피어들에게 NetConnection 정보 전송 요청
		std::cout << "InfoSent" << '\n';
		ctx_.state = MatchState::WaitPeerReady;
		break;

	case MatchState::WaitPeerReady:
	// 모든 피어들의 피어간 연결 성공 패킷이 다 도착할 때까지 대기
	// 일정 시간이 지나면 timeout으로 Failed 처리
		std::cout << "WaitPeerReady" << '\n';
		if(timer_ >= MatchConstants::CONNECT_TIMEOUT_SEC)
			ctx_.state = MatchState::Failed;
		else
			ctx_.state = MatchState::Done;
		break;

	case MatchState::Failed:
		std::cout << "Failed" << '\n';
		break;

	case MatchState::Done:
		std::cout << "Done" << '\n';
		break;
	}
}