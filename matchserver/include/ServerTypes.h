#ifndef SERVER_TYPES_H
#define SERVER_TYPES_H

using MatchId = uint32_t;
using RequestId = uint32_t;
using SessionId = uint32_t;
using ServerTimeMs = uint64_t;

constexpr uint16_t MAX_PACKET_SIZE{ 1024 };
using RawBuffer = std::array<uint8_t, MAX_PACKET_SIZE>;



#endif // !SERVER_TYPES_H