#ifndef MATCH_MANAGER_H
#define MATCH_MANAGER_H

#include <tomato/containers/SPSCQueue.h>
#include "Match.h"

class MatchManager
{
public:

	/////////////ONLY FOR TEST////////////
	void AddMatchRequest(int playerId);
	//////////////////////////////////////

	void Update(float dt);

	bool CheckPopulation();
	bool CreateMatchContext(MatchContext& ctx);
	//void HandleMatchAction();
	void EmitMatchResult(MatchEvent& evt);

private:
	int nextMatchID_ = 1;
	tomato::SPSCQueue<MatchRequest, 128> MatchRequestQueue;
	tomato::SPSCQueue<MatchEvent, 128> MatchResultQueue;
	std::vector<Match> matches;
};

#endif // !MATCH_MANAGER_H