#ifndef TOMATO_INPUTNETMESSAGE_H
#define TOMATO_INPUTNETMESSAGE_H

#include "tomato/net/NetMessage.h"
#include "tomato/services/InputTypes.h"

namespace tomato
{
    class NetBitReader;
    class NetBitWriter;
    class Engine;

    struct InputNetMessage : public NetMessage
    {
    private:
        void Serialize(NetBitWriter& writer, Engine& engine) override;

        void Deserialize(NetBitReader& reader) override;
        void Handler(Engine &engine, SocketAddress &fromAddr) override;

        InputRecord inputRecord;
    };
}

#endif //TOMATO_INPUTNETMESSAGE_H
