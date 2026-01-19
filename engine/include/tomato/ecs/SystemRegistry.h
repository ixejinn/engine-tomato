#ifndef TOMATO_SYSTEMREGISTRY_H
#define TOMATO_SYSTEMREGISTRY_H

#include <functional>
#include <memory>
#include <vector>
#include "tomato/tomato_sim.h"

namespace tomato
{
    class System;

    using Factory = std::function<std::unique_ptr<System>()>;

    class SystemRegistry
    {
    private:
        SystemRegistry() = default;

    public:
        ~SystemRegistry() = default;

        SystemRegistry& operator=(const SystemRegistry&) = delete;

        void RegisterFactory(SystemPhase phase, Factory&& factory);
        const std::vector<Factory>& GetFactory(SystemPhase phase);

        static SystemRegistry& GetInstance()
        {
            static SystemRegistry instance;
            return instance;
        }

    private:
        std::array<std::vector<Factory>, ToIndex(SystemPhase::COUNT)> factories_{};
    };

    struct SystemRegistryEntry
    {
        SystemRegistryEntry(const SystemPhase phase, Factory&& factory)
        {
            SystemRegistry::GetInstance().RegisterFactory(phase, std::move(factory));
        }
    };
}

#define REGISTER_SYSTEM(PHASE, CLASS)\
namespace { static tomato::SystemRegistryEntry CLASS##Entry{PHASE, []() { return std::make_unique<tomato::CLASS>(); }}; }

#endif //TOMATO_SYSTEMREGISTRY_H
