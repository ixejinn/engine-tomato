#include "MatchManager.h"


void MatchManager::AddMatchRequest(int playerId)
{
	MatchRequest mRequest;
	mRequest.requestId = playerId;
	mRequest.enqueueTime = 0.f;
	mRequest.retryCount = 0;

	MatchRequestQueue.Emplace(mRequest);
}

void MatchManager::Update(float dt)
{
	if (CheckPopulation())
	{
		MatchContext ctx;
		CreateMatchContext(ctx);
		matches.emplace_back(Match(ctx));

		std::cout << "Enough\n";

		for (auto c : ctx.players)
			std::cout << c.requestId << " ";
	}

	for (auto& m : matches)
	{
		if (m.GetState() == MatchState::Failed)
		{
			MatchEvent evt;
			evt.matchId = m.GetMatchId();
			evt.type = MatchEventType::Failed;
			EmitMatchResult(evt);

			for (auto& m : matches)
			{
				//map이나 list 같은걸로 바꿔야겠다 삭제가 잦으니까
				if (evt.matchId == m.GetMatchId())
					matches.pop_back();
				//일단 1개로 테스트
			}
		}

		if (m.GetState() == MatchState::Done)
		{
			MatchEvent evt;
			evt.matchId = m.GetMatchId();
			evt.type = MatchEventType::AllReady;
			EmitMatchResult(evt);

			for (auto& m : matches)
			{
				if (evt.matchId == m.GetMatchId())
					matches.pop_back();
			}
		}
		m.Update(dt);
	}
}

bool MatchManager::CheckPopulation()
{
	if (MatchRequestQueue.Size() >= MatchConstants::MAX_MATCH_PLAYER)
		return true;
	return false;
}

bool MatchManager::CreateMatchContext(MatchContext& ctx)
{
	ctx.matchId = nextMatchID_++;
	ctx.state = MatchState::Init;

	MatchRequest out;
	for (int i = 0; i < MatchConstants::MAX_MATCH_PLAYER; i++)
	{
		if (MatchRequestQueue.Dequeue(out))
			ctx.players[i] = out;
	}

	return true;
}

void MatchManager::EmitMatchResult(MatchEvent& evt)
{
	switch (evt.type)
	{
	case MatchEventType::TimeOut:
	case MatchEventType::Failed:
		MatchResultQueue.Emplace(evt);
		break;
	}
}