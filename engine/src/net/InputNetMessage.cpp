#include "tomato/net/InputNetMessage.h"
#include "tomato/net/NetBitReader.h"
#include "tomato/net/NetBitWriter.h"
#include "tomato/Engine.h"
#include "tomato/services/network/SocketAddress.h"
//#include "tomato/services/network/NetworkService.h"

#include <limits>

namespace tomato
{
    void InputNetMessage::Serialize(NetBitWriter& writer, Engine& engine)
    {
        uint32_t tick = engine.GetTick();
        inputRecord = engine.GetInputTimeline()[engine.GetNetworkService().GetPlayerID()][tick];
        writer.WriteInt(tick, std::numeric_limits<uint32_t>::max());
        writer.WriteInt(static_cast<uint16_t>(inputRecord.keydown), static_cast<uint32_t>(InputAction::END));
        writer.WriteInt(static_cast<uint16_t>(inputRecord.key), static_cast<uint32_t>(InputAction::END));
    }

    void InputNetMessage::Deserialize(NetBitReader& reader)
    {
        reader.ReadInt(inputRecord.tick, std::numeric_limits<uint32_t>::max());

        uint16_t value = 0;
        reader.ReadInt(value, uint32_t(InputAction::END));
        inputRecord.keydown = static_cast<InputAction>(value);
        reader.ReadInt(value, uint32_t(InputAction::END));
        inputRecord.key = static_cast<InputAction>(value);
    }

    void InputNetMessage::Handler(Engine& engine, SocketAddress& fromAddr)
    {
        engine.SetInputData(engine.GetNetworkService().GetPlayerID(fromAddr), inputRecord);
        if (engine.GetLatestTick() > inputRecord.tick)
            engine.SetLatestTick(inputRecord.tick);

        //auto tmp = static_cast<uint16_t>(inputRecord.key);
        //std::cout << "InputNetMessage::Handler [" << engine.GetNetworkService().GetPlayerID(fromAddr) << "] " << inputRecord.tick << " : " << tmp << "\n";
    }
}