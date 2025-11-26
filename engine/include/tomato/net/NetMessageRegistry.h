#ifndef TOMATO_NETMESSAGEREGISTRY_H
#define TOMATO_NETMESSAGEREGISTRY_H

#include <cstdint>
#include <functional>

namespace tomato
{
    class Engine;
    class NetBitReader;

    using Decode = std::function<void(Engine&, NetBitReader&)>;

    // Registry 묶는 건?
    class NetMessageRegistry
    {
    private:
        NetMessageRegistry() = default;

    public:
        NetMessageRegistry(const NetMessageRegistry&) = delete;
        NetMessageRegistry& operator=(const NetMessageRegistry&) = delete;

        void RegisterNetMessageDecode(uint32_t type, Decode&& decode);

        Decode GetDecode(uint32_t type);

        static NetMessageRegistry& GetInstance()
        {
            static NetMessageRegistry instance;
            return instance;
        }

    private:

    };
}

#endif //TOMATO_NETMESSAGEREGISTRY_H
