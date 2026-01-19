#include "tomato/ecs/SystemRegistry.h"
#include "tomato/ecs/systems/System.h"

namespace tomato
{
    void SystemRegistry::RegisterFactory(const SystemPhase phase, Factory&& factory)
    {
        factories_[ToIndex(phase)].push_back(std::move(factory));
    }

    const std::vector<Factory>& SystemRegistry::GetFactory(const SystemPhase phase)
    {
        return factories_[ToIndex(phase)];
    }
}