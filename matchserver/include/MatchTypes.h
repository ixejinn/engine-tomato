#ifndef MATCH_TYPES_H
#define MATCH_TYPES_H

#include <typeinfo>
#include <vector>
#include "ServerTypes.h"

namespace MatchConstants
{
	constexpr int	MAX_MATCH_PLAYER = 4;
	constexpr float	CONNECT_TIMEOUT_SEC = 2.0f;
}

enum class MatchRequestAction : uint8_t
{
	NONE,
	Enqueue,
	Cancel
};

struct MatchRequestCommand
{
	SessionId sessionId;
	RequestId requestId;
	MatchRequestAction action;
};

struct MatchRequest
{
	SessionId sessionId;
	RequestId requestId;
	ServerTimeMs enqueueTime;
	int retryCount;
};

enum class MatchState;
struct MatchContext
{
	MatchId matchId;
	MatchState state;
	MatchRequest players[MatchConstants::MAX_MATCH_PLAYER];
};

enum class MatchEventType
{
	AllReady,
	Failed,
};

struct MatchEvent
{
	MatchId matchId;
	MatchEventType type;
};

#endif // !MATCH_TYPES_H