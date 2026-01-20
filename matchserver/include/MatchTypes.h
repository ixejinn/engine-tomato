#ifndef MATCH_TYPES_H
#define MATCH_TYPES_H

#include <typeinfo>
#include <vector>

namespace MatchConstants
{
	constexpr int	MAX_MATCH_PLAYER = 4;
	constexpr float	CONNECT_TIMEOUT_SEC = 2.0f;
}

struct MatchRequest
{
	uint64_t requestId;
	float enqueueTime;
	int retryCount;
	//Session session;
};

enum class MatchState;
struct MatchContext
{
	uint32_t matchId;
	MatchState state;
	//Session players[MAX_MATCH_PLAYER];
	//for test
	MatchRequest players[MatchConstants::MAX_MATCH_PLAYER];
};

enum class MatchEventType
{
	AllReady,
	TimeOut,
	Failed,
};

struct MatchEvent
{
	uint32_t matchId;
	MatchEventType type;
};

#endif // !MATCH_TYPES_H