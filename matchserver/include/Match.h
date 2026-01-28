#ifndef MATCH_H
#define MATCH_H

#include "MatchTypes.h"

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

	void Update(float dt);

	MatchState GetState() const { return ctx_.state; }
	MatchId GetMatchId() const { return ctx_.matchId; }
	const MatchRequest* GetMatchRequest() const { return ctx_.players; }
private:
	MatchContext ctx_;
	float timer_; //check for waiting, timeout
};

#endif // !MATCH_H