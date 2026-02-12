#include "tomato/net/NetMessageRegistry.h"

namespace tomato
{
    void NetMessageRegistry::RegisterFactory(const NetMessageType type, NetMessageFactory&& factory)
    {
        factories_[type] = std::move(factory);
    }

    const NetMessageFactory& NetMessageRegistry::GetFactory(const NetMessageType type)
    {
        return factories_[type];
    }
}