#include "tomato/net/InputNetMessage.h"
#include "tomato/net/NetBitReader.h"
#include "tomato/net/NetBitWriter.h"
#include "tomato/Engine.h"
#include "tomato/services/network/SocketAddress.h"
#include <limits>

#include "tomato/RegistryEntry.h"
REGISTER_NET_MESSAGE(tomato::NetMessageType::INPUT, InputNetMessage)

namespace tomato
{
    void InputNetMessage::Serialize(NetBitWriter& writer)
    {
        writer.WriteInt(tick, std::numeric_limits<uint32_t>::max());
        writer.WriteInt(static_cast<uint16_t>(inputRecord.down), static_cast<uint16_t>(InputIntent::COUNT));
        writer.WriteInt(static_cast<uint16_t>(inputRecord.held), static_cast<uint16_t>(InputIntent::COUNT));
    }

    void InputNetMessage::Build(Engine& engine)
    {
        tick = engine.GetTick();
        inputRecord = engine.GetInputTimeline()[engine.GetNetworkService().GetPlayerID()][tick];
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

    void InputNetMessage::Apply(SocketAddress& fromAddr, Engine& engine)
    {
        engine.SetInputData(engine.GetNetworkService().GetPlayerID(fromAddr), inputRecord);
        engine.SetLatestTick(inputRecord.tick);
    }
}