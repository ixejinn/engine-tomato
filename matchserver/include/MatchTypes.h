#ifndef MATCH_TYPES_H
#define MATCH_TYPES_H

#include <typeinfo>
#include <vector>
#include <tomato/services/network/TCPSocket.h>
#include "ServerTypes.h"
#include "PacketTypes.h"

namespace MatchConstants
{
	constexpr int			MAX_MATCH_PLAYER = 4;
	constexpr float			CONNECT_TIMEOUT_SEC = 2.0f;
	constexpr ServerTimeMs	START_SERVER_TIME = 10000;
}

enum class MatchRequestAction : uint8_t
{
	NONE,
	Enqueue,
	Cancel,
	Success,
	Failed,

	COUNT
};

struct MatchRequestCommand
{
	tomato::TCPSocketPtr socket;
	MatchId matchId;
	MatchRequestAction action;
};

struct SendRequestCommand
{
	tomato::TCPSocketPtr socket;
	TCPHeader header;
	uint8_t data;
};

struct MatchRequest
{
	tomato::TCPSocketPtr socket;
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

enum class MatchUpdateResult
{
	None,
	ReadyToStart,
	Failed,

	COUNT
};

#endif // !MATCH_TYPES_H