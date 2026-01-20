#ifndef MATCH_H
#define MATCH_H

#include "MatchTypes.h"

enum class MatchState
{
	Init,
	InfoSent,
	WaitPeerReady,
	Done,
	Failed
};

class Match
{
public:
	explicit Match(const MatchContext& ctx);

	void Update(float dt);

	MatchState GetState() const { return ctx_.state; }
	uint32_t GetMatchId() const { return ctx_.matchId; }
private:
	MatchContext ctx_;
	float timer_; //check for waiting, timeout
};

#endif // !MATCH_H