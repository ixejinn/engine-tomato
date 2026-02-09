#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H

#include <unordered_map>
#include <queue>
#include <vector>
#include <tomato/containers/SPSCQueue.h>

#include "Match.h"
#include "PacketTypes.h"

class MatchManager
{
public:
	MatchManager(tomato::SPSCQueue<MatchRequestCommand, 128>& requestQ, tomato::SPSCQueue<SendCommandPtr, 256>& netSendRequestQ)
		: MatchRequestQueue(requestQ), NetSendRequestQueue(netSendRequestQ) {};

	/////////////ONLY FOR TEST////////////
	void AddMatchRequestQueue(SessionId sessionId, RequestId reqId);
	//////////////////////////////////////

	void Update(float dt);

	void ProcessMatchRequest();
	void HandleEnqueue(const SessionId& client);
	void HandleCancel(const SessionId& client);
	void HandleIntroResult(const SessionId&, MatchId& matchId, const int& set);

	bool CheckPopulation();
	bool GetMatchRequestFromPQ(MatchRequest& req);
	bool CreateMatchContext(MatchContext& ctx);

	void HandleSendRequest(tomato::TCPSocketPtr& socket, uint8_t* inData);
	void ProcessMatchResult(float dt);
	void ReQueing(const MatchId& matchId);

private:
	struct Compare {
		bool operator()(const MatchRequest& a, const MatchRequest& b) const {
			if (a.retryCount != b.retryCount)
				return a.retryCount < b.retryCount;

			return a.enqueueTime > b.enqueueTime;
		}
	};

private:
	MatchId nextMatchID_ = 1;

	tomato::SPSCQueue<SendCommandPtr, 256>& NetSendRequestQueue;
	tomato::SPSCQueue<MatchRequestCommand, 128>& MatchRequestQueue;

	std::unordered_map<SessionId, MatchRequest> requests;
	std::priority_queue<MatchRequest, std::vector<MatchRequest>, Compare> pq;
	std::unordered_map<MatchId, Match> matches;
};

#endif // !MATCH_MANAGER_H