#include "MatchManager.h"
#include <chrono>

void MatchManager::AddMatchRequestQueue(SessionId sessionId, RequestId reqId)
{
	MatchRequestCommand reqCommand;
	reqCommand.sessionId = sessionId;
	reqCommand.requestId = reqId;
	reqCommand.action = MatchRequestAction::Enqueue;

	MatchRequestQueue.Emplace(reqCommand);
}

void MatchManager::Update(float dt)
{
	ProcessMatchRequest();

	// Create a match when people get together
	if (CheckPopulation())
	{
		MatchContext ctx;
		CreateMatchContext(ctx);
		matches.try_emplace(ctx.matchId, ctx);

		for (const auto& c : ctx.players)
			std::cout << c.requestId << " ";
	}
	
	// Update the state of all matches (Separating as a function)
	for (auto it = matches.begin(); it != matches.end();)
	{
		it->second.Update(dt);

		switch (it->second.GetState())
		{
		case MatchState::AllReady:
			//SendPacket for game start
			//then, Remove from matches
			NetSendRequestQueue.Emplace(static_cast<uint8_t>(PacketHeader::MATCH_START));
			it = matches.erase(it);
			break;

		// When peers can't connect or time out
		case MatchState::Failed:
			// Remove from matches after ReQueing
			ReQueing(it->first);
			it = matches.erase(it);

			std::cout << "ReQueing\n";
			break;

		default:
			++it;
			break;
		}
	}	
}

bool MatchManager::CheckPopulation()
{
	if (requests.size() >= MatchConstants::MAX_MATCH_PLAYER)
		return true;
	return false;
}

void MatchManager::ProcessMatchRequest()
{
	while (!MatchRequestQueue.Empty())
	{
		MatchRequestCommand reqCommand;
		MatchRequestQueue.Dequeue(reqCommand);

		if (reqCommand.action == MatchRequestAction::Enqueue)
			HandleEnqueue(reqCommand.sessionId, reqCommand.requestId);
		
		if (reqCommand.action == MatchRequestAction::Cancel)
			HandleCancel(reqCommand.sessionId, reqCommand.requestId);
	}
}

void MatchManager::HandleEnqueue(SessionId sessionId, RequestId reqId)
{
	MatchRequest mRequest;
	mRequest.sessionId = sessionId;
	mRequest.requestId = reqId;
	mRequest.enqueueTime = 0;
	//duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	mRequest.retryCount = 0;

	requests.try_emplace(sessionId, mRequest);
	pq.emplace(mRequest);
}

void MatchManager::HandleCancel(SessionId sessionId, RequestId reqId)
{
	auto it = requests.find(sessionId);
	if (it != requests.end())
	{
		if(it->second.requestId == reqId)
			requests.erase(it);
	}
}

bool MatchManager::GetMatchRequestFromPQ(MatchRequest& req)
{
	while (!pq.empty())
	{
		MatchRequest item = pq.top();
		pq.pop();

		if (!requests.contains(item.sessionId))
			continue; // lazy deletion

		else
		{
			req = item;
			return true;
		}
	}
	return false;
}

bool MatchManager::CreateMatchContext(MatchContext& ctx)
{
	ctx.matchId = nextMatchID_++;
	ctx.state = MatchState::Init;

	MatchRequest out;
	for (int i = 0; i < MatchConstants::MAX_MATCH_PLAYER; i++)
	{
		if (GetMatchRequestFromPQ(out))
		{
			ctx.players[i] = out;
			requests.erase(out.sessionId);
		}
	}

	return true;
}

void MatchManager::EmitMatchResult(MatchEvent& evt)
{
	switch (evt.type)
	{
	case MatchEventType::Failed:
		MatchResultQueue.Emplace(evt);
		break;
	case MatchEventType::AllReady:
		MatchResultQueue.Emplace(evt);
		break;
	}
}

void MatchManager::ReQueing(MatchId matchId)
{
	auto it = matches.find(matchId);
	if (it != matches.end())
	{
		const MatchRequest* request = it->second.GetMatchRequest();
		
		for (int i = 0; i < MatchConstants::MAX_MATCH_PLAYER; i++)
		{
			MatchRequest newRequest;
			newRequest.sessionId = (request + i)->sessionId;
			newRequest.enqueueTime = duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
			newRequest.requestId = (request + i)->requestId;
			newRequest.retryCount = (request + i)->retryCount + 1;
			
			if (newRequest.retryCount > 3)
				continue;
			//HandleEnqueue(newRequest.sessionId, newRequest.requestId);
			requests.try_emplace(newRequest.sessionId, newRequest.requestId);
			pq.emplace(newRequest);
		}
	}
}