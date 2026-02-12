#include "tomato/ecs/SystemRegistry.h"
#include "tomato/ecs/systems/System.h"

namespace tomato
{
    void SystemRegistry::RegisterFactory(const SystemPhase phase, SystemFactory&& factory)
    {
        factories_[phase].push_back(std::move(factory));
    }

    const std::vector<SystemFactory>& SystemRegistry::GetFactory(const SystemPhase phase)
    {
        return factories_[phase];
    }
}