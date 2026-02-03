#include "MatchManager.h"
#include <chrono>

void MatchManager::AddMatchRequestQueue(SessionId sessionId, RequestId reqId)
{
	MatchRequestCommand reqCommand{
		.matchId = 0,
		.action = MatchRequestAction::Enqueue
	};

	MatchRequestQueue.Emplace(reqCommand);
}
/////////////////////////////////////////////////////////////////////////////////


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
			std::cout << c.sessionId << " ";
	}
	
	ProcessMatchResult(dt);
}

void MatchManager::ProcessMatchRequest()
{
	while (!MatchRequestQueue.Empty())
	{
		MatchRequestCommand reqCommand;
		MatchRequestQueue.Dequeue(reqCommand);

		if (reqCommand.action == MatchRequestAction::Enqueue)
			HandleEnqueue(reqCommand.socket);
		
		if (reqCommand.action == MatchRequestAction::Cancel)
			HandleCancel(reqCommand.socket);

		if (reqCommand.action == MatchRequestAction::Success)
			HandleIntroResult(reqCommand.socket, reqCommand.matchId, 1);

		if (reqCommand.action == MatchRequestAction::Failed)
			HandleIntroResult(reqCommand.socket, reqCommand.matchId, 0);
	}
}

void MatchManager::HandleEnqueue(tomato::TCPSocketPtr client)
{
	MatchRequest mRequest{
		.socket = client,
		.sessionId = 0,
		.requestId = 0,
		.enqueueTime = 0,
		//duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
		.retryCount = 0,
	};

	requests.try_emplace(client, mRequest);
	pq.emplace(mRequest);
}

void MatchManager::HandleCancel(tomato::TCPSocketPtr client)
{
	/* @TODO:
	* Match cancellation during active matching is not fully handled.
	* For now, we only remove the request from the map.
	* Proper cancellation logic is needed when a match is already being created.
	*/

	auto it = requests.find(client);
	if (it != requests.end())
	{
		requests.erase(it);
	}
}

void MatchManager::HandleIntroResult(tomato::TCPSocketPtr client, MatchId matchId, int set)
{
	auto it = matches.find(matchId);
	if (it != matches.end())
	{
		int idx = it->second.GetPlayerId(client);
		if (idx < 0) return;
		it->second.SetPeerAck(idx, set);
	}
}

bool MatchManager::CheckPopulation()
{
	if (requests.size() >= MatchConstants::MAX_MATCH_PLAYER)
		return true;
	return false;
}

bool MatchManager::GetMatchRequestFromPQ(MatchRequest& req)
{
	while (!pq.empty())
	{
		MatchRequest item = pq.top();
		pq.pop();

		if (!requests.contains(item.socket))
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
			requests.erase(out.socket);
		}
	}

	return true;
}

void HandleSendRequest(tomato::TCPSocketPtr socket, uint8_t* inData)
{

}

void MatchManager::ProcessMatchResult(float dt)
{
	// Update the state of all matches (Separating as a function)
	for (auto it = matches.begin(); it != matches.end();)
	{
		MatchUpdateResult res = it->second.Update(dt, NetSendRequestQueue);

		switch (res)
		{
		case MatchUpdateResult::None:
		{
			++it;
			break;
			//const MatchRequest* req = it->second.GetMatchRequest();
			//const tomato::NetConnection* conn = it->second.GetNetConnection();
			//for (int i = 0; i < MatchConstants::MAX_MATCH_PLAYER; i++)
			//{
			//	SendRequestCommand sendCmd{ (req + i)->socket, static_cast<uint8_t>(TCPPacketType::MATCH_INTRO) };
			//	NetSendRequestQueue.Emplace(sendCmd);
			//}
			//break;
		}

		case MatchUpdateResult::ReadyToStart:
		{
			//SendPacket for game start
			//then, Remove from matches
			ServerTimeMs startServerTime = MatchConstants::START_SERVER_TIME;

			const MatchRequest* req = it->second.GetMatchRequest();
			TCPHeader header{ sizeof(TCPHeader), TCPPacketType::MATCH_START};
			uint8_t data = static_cast<uint8_t>(startServerTime);

			for (int i = 0; i < MatchConstants::MAX_MATCH_PLAYER; i++)
			{
				SendRequestCommand sendCmd{ (req + i)->socket, header, data };
				NetSendRequestQueue.Emplace(sendCmd);
			}
			it = matches.erase(it);
			break;
		}

			// When peers can't connect or time out
		case MatchUpdateResult::Failed:
		{
			// Remove from matches after ReQueing
			ReQueing(it->first);
			it = matches.erase(it);

			std::cout << "ReQueing\n";
			break;
		}
		}
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
			MatchRequest reRequest
			{
				.sessionId = (request + i)->sessionId,
				.requestId = (request + i)->requestId,
				.enqueueTime = static_cast<ServerTimeMs>(
					duration_cast<std::chrono::milliseconds>(
						std::chrono::steady_clock::now().time_since_epoch()
					).count()
				),
				.retryCount = (request + i)->retryCount + 1,
			};
			
			if (reRequest.retryCount > 3)
				continue;
			
			requests.try_emplace(reRequest.socket, reRequest);
			pq.emplace(reRequest);
		}
	}
}