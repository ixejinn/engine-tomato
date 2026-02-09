#include "tomato/net/InputNetMessage.h"
#include "tomato/net/NetBitReader.h"
#include "tomato/net/NetBitWriter.h"
#include "tomato/Engine.h"
#include "tomato/services/network/SocketAddress.h"

#include <limits>

namespace tomato
{
    void InputNetMessage::Serialize(NetBitWriter& writer, Engine& engine)
    {
        uint32_t tick = engine.GetTick();
        inputRecord = engine.GetInputTimeline()[engine.GetNetworkService().GetPlayerID()][tick];
        writer.WriteInt(tick, std::numeric_limits<uint32_t>::max());
        writer.WriteInt(static_cast<uint16_t>(inputRecord.down), static_cast<uint16_t>(InputIntent::COUNT));
        writer.WriteInt(static_cast<uint16_t>(inputRecord.held), static_cast<uint16_t>(InputIntent::COUNT));
    }

    void InputNetMessage::Deserialize(NetBitReader& reader)
    {
        reader.ReadInt(inputRecord.tick, std::numeric_limits<uint32_t>::max());

        uint16_t value = 0;
        reader.ReadInt(value, uint16_t(InputIntent::COUNT));
        inputRecord.down = static_cast<InputIntent>(value);
        reader.ReadInt(value, uint16_t(InputIntent::COUNT));
        inputRecord.held = static_cast<InputIntent>(value);
    }

    void InputNetMessage::Handler(Engine& engine, SocketAddress& fromAddr)
    {
        engine.SetInputData(engine.GetNetworkService().GetPlayerID(fromAddr), inputRecord);
        engine.SetLatestTick(inputRecord.tick);
    }
}