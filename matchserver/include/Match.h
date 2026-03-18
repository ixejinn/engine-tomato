#ifndef MATCH_H
#define MATCH_H

#include "MatchTypes.h"
#include <bitset>
#include <tomato/containers/SPSCQueue.h>
#include <tomato/services/network/NetConnection.h>

enum class MatchState
{
	Init,
	InfoSent,
	WaitPeerReady,
	AllReady,
	Failed,
	Processing,
};

class Match
{
public:
	explicit Match(const MatchContext& ctx);

	MatchUpdateResult Update(float dt, tomato::SPSCQueue<SendCommandPtr, 256>& sendRequestQ);

	MatchState CollectNetConnection(const MatchRequest* matchRequest);
	MatchState RequestToSendNetConnection(tomato::SPSCQueue<SendCommandPtr, 256>& sendRequestQ, const MatchRequest* matchRequest);
	
	void SetPeerAck(int idx, int set);
	
	const int GetPlayerId(const SessionId& client) const;
	MatchState GetState() const { return ctx_.state; }
	MatchId GetMatchId() const { return ctx_.matchId; }
	const MatchRequest* GetMatchRequest() const { return ctx_.players; }
	const tomato::NetConnection* GetNetConnection() const { return conn; }

private:
	MatchContext ctx_;
	tomato::NetConnection conn[MatchConstants::MAX_MATCH_PLAYER];
	
	std::bitset<MatchConstants::MAX_MATCH_PLAYER> peerAck;

	float timer_{ 0.f }; //check for waiting, timeout
};

#endif // !MATCH_H