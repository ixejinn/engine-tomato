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
         * @brief Reads a message from the given bit reader and dispatches it.
         * @param reader Bit reader containing the message payload.
         * @param engine Engine instance to apply/consume the message.
         * @param fromAddr Source address of the received datagram.
         *
         * Deserialize()를 호출해 멤버 필드를 채운 뒤,
         * Handler()를 호출해 처리한다.
         */
        void Read(NetBitReader& reader, Engine& engine, SocketAddress& fromAddr)
        {
            Deserialize(reader);
            Handler(engine, fromAddr);
        }

        /**
         * @brief Writes a message to the given bit writer.
         * @param writer Bit writer to append the serialized payload.
         * @param engine Engine instance used as a data source for serialization.
         *
         * Serialize()를 호출해 전달할 데이터를 payload로 인코딩한다.
         */
        void Write(NetBitWriter& writer, Engine& engine)
        {
            Serialize(writer, engine);
        }

    private:
        /**
         * @brief Serializes this message into a bit-stream.
         * @warning Must remains symmetric with Deserialize().
         */
        virtual void Serialize(NetBitWriter&, Engine&) = 0;

        /**
         * @brief Deserializes this message from a bit-stream.
         * @warning Must remains symmetric with Serialize().
         */
        virtual void Deserialize(NetBitReader&) = 0;
        virtual void Handler(Engine&, SocketAddress&) = 0;
    };
}

#endif //TOMATO_NETMESSAGE_H
