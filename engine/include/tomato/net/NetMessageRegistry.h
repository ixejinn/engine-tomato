#ifndef TOMATO_NETMESSAGEREGISTRY_H
#define TOMATO_NETMESSAGEREGISTRY_H

#include <cstdint>
#include <functional>
#include <memory>
#include "tomato/containers/EnumArray.h"
#include "tomato/net/tomato_message_type.h"

namespace tomato
{
    struct NetMessage;

    using NetMessageFactory = std::function<std::unique_ptr<NetMessage>()>;

    class NetMessageRegistry
    {
    private:
        NetMessageRegistry() = default;

    public:
        ~NetMessageRegistry() = default;

        NetMessageRegistry(const NetMessageRegistry&) = delete;
        NetMessageRegistry& operator=(const NetMessageRegistry&) = delete;

        void RegisterFactory(NetMessageType type, NetMessageFactory&& factory);
        const NetMessageFactory& GetFactory(NetMessageType type);

        static NetMessageRegistry& GetInstance()
        {
            static NetMessageRegistry instance;
            return instance;
        }

    private:
        EnumArray<NetMessageType, NetMessageFactory> factories_{};
    };
}

#endif //TOMATO_NETMESSAGEREGISTRY_H
