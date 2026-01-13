#ifndef TOMATO_SYSTEMREGISTRY_H
#define TOMATO_SYSTEMREGISTRY_H

#include <functional>
#include <memory>
#include <vector>

namespace tomato
{
    class System;

    enum SystemType : uint8_t
    {
        CONTROLLER = 0,
        INTEGRATOR,
        COLLISION,
        GAMERULE,
        SPAWNER,
        RENDER,
        CAMERA,
    };

    using Factory = std::function<std::unique_ptr<System>()>;

    class SystemRegistry
    {
    private:
        SystemRegistry() = default;
        ~SystemRegistry();

    public:
        SystemRegistry(const SystemRegistry&) = delete;
        SystemRegistry& operator=(const SystemRegistry&) = delete;

        void RegisterSystemFactory(SystemType type, Factory&& factory);

        const std::vector<Factory>& GetFactory(SystemType type);

        static SystemRegistry& GetInstance()
        {
            static SystemRegistry instance;
            return instance;
        }

    private:
        // map<SystemType, std::vector<Factory>>로 바꾸는 건 어떨지
        std::vector<Factory> controllers_;
        std::vector<Factory> integrators_;
        std::vector<Factory> collisions_;
        std::vector<Factory> gameRules_;
        std::vector<Factory> spawners_;

        std::vector<Factory> renderers_;
        std::vector<Factory> camera_;
    };

    struct SystemRegistryEntry
    {
        SystemRegistryEntry(const SystemType type, Factory&& factory)
        {
            SystemRegistry::GetInstance().RegisterSystemFactory(type, std::move(factory));
        }
    };
}

#define REGISTER_SYSTEM(TYPE, CLASS)\
namespace { static tomato::SystemRegistryEntry CLASS##Entry{TYPE, []() { return std::make_unique<tomato::CLASS>(); }}; }

#endif //TOMATO_SYSTEMREGISTRY_H
