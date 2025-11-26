#ifndef TOMATO_NETMESSAGE_H
#define TOMATO_NETMESSAGE_H

namespace tomato
{
    class Engine;
    class NetBitReader;
    class NetBitWriter;
    class SocketAddress;

    struct NetMessage
    {
    public:
        void Read(NetBitReader& reader, Engine& engine, SocketAddress& fromAddr)
        {
            Deserialize(reader);
            Handler(engine, fromAddr);
        }

        void Write(NetBitWriter& writer, Engine& engine)
        {
            Serialize(writer, engine);
        }

    private:
        virtual void Serialize(NetBitWriter&, Engine&) = 0;

        virtual void Deserialize(NetBitReader&) = 0;
        virtual void Handler(Engine&, SocketAddress&) = 0;
    };
}

#endif //TOMATO_NETMESSAGE_H
