#ifndef TOMATO_NETMESSAGE_H
#define TOMATO_NETMESSAGE_H

namespace tomato
{
    class Engine;
    class NetBitReader;
    class NetBitWriter;
    class SocketAddress;

    /**
     * @brief Base interface for network messages.
     *
     * @warning Serialize() and Deserialize() must remain strictly symmetric.
     */
    struct NetMessage
    {
    public:
        /**
         * @brief Deserializes the payload and applies it to the engine.
         * @param reader Bit reader containing the message payload.
         * @param engine Engine instance to apply the message.
         * @param fromAddr Source address of the received datagram.
         */
        void Read(NetBitReader& reader, Engine& engine, SocketAddress& fromAddr)
        {
            Deserialize(reader);
            Apply(fromAddr, engine);
        }

        /**
         * @brief Deserializes the payload and applies it without engine context.
         * @param reader Bit reader containing the message payload.
         * @param fromAddr Source address of the received datagram.
         */
        void Read(NetBitReader& reader, SocketAddress& fromAddr)
        {
            Deserialize(reader);
            Apply(fromAddr);
        }

        /**
         * @brief Builds message data from the engine and serializes it.
         * @param writer Bit writer to append the serialized payload.
         * @param engine Engine instance used as a data source for serialization.
         *
         * Calls Build() followed by Serialize().
         */
        void Write(NetBitWriter& writer, Engine& engine)
        {
            Build(engine);
            Serialize(writer);
        }

        /**
         * @brief Serializes the message without engine context.
         * @param writer Bit writer to append the serialized payload.
         */
        void Write(NetBitWriter& writer)
        {
            Build();
            Serialize(writer);
        }

    private:
        /**
         * @brief Serializes member variables into a bit-stream.
         * @warning Must remains symmetric with Deserialize().
         */
        virtual void Serialize(NetBitWriter&) = 0;

        virtual void Build(Engine&) {};
        virtual void Build() {};

        /**
         * @brief Deserializes member variables from a bit-stream.
         * @warning Must remains symmetric with Serialize().
         */
        virtual void Deserialize(NetBitReader&) = 0;

        virtual void Apply(SocketAddress&, Engine&) {};
        virtual void Apply(SocketAddress&) {};
    };
}

#endif //TOMATO_NETMESSAGE_H
