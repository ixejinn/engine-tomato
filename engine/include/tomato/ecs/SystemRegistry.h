#ifndef TOMATO_SYSTEMREGISTRY_H
#define TOMATO_SYSTEMREGISTRY_H

#include <functional>
#include <memory>
#include <vector>

namespace tomato
{
    class System;
    class CharacterInputHistory;

    enum SystemType : uint8_t
    {
        CONTROLLER = 0,
        INTEGRATOR,
        COLLISION,
        GAMERULE,
        SPAWNER,
        RENDER
    };

    using Factory = std::function<std::unique_ptr<System>()>;
    using ControllerFactory = std::function<std::unique_ptr<System>(std::vector<CharacterInputHistory>&)>;

    class SystemRegistry
    {
    private:
        std::vector<ControllerFactory> controllers_;
        std::vector<Factory> integrators_;
        std::vector<Factory> collisions_;
        std::vector<Factory> gameRules_;
        std::vector<Factory> spawners_;

        std::vector<Factory> renderers_;

        SystemRegistry() = default;
        ~SystemRegistry();

    public:
        SystemRegistry(const SystemRegistry&) = delete;
        SystemRegistry& operator=(const SystemRegistry&) = delete;

        static SystemRegistry& GetInstance()
        {
            static SystemRegistry instance;
            return instance;
        }

        void RegisterSystemFactory(ControllerFactory&& factory);
        void RegisterSystemFactory(const SystemType& type, Factory&& factory);

        const std::vector<ControllerFactory>& GetControllerFactory() { return controllers_; }
        const std::vector<Factory>& GetFactory(const SystemType& type);
    };

    struct SystemRegistryEntry
    {
        SystemRegistryEntry(ControllerFactory&& factory) { SystemRegistry::GetInstance().RegisterSystemFactory(std::move(factory)); }
        SystemRegistryEntry(const SystemType& type, Factory&& factory) { SystemRegistry::GetInstance().RegisterSystemFactory(type, std::move(factory)); }
    };
}

#define REGISTER_SYSTEM(TYPE, CLASS)\
namespace { static tomato::SystemRegistryEntry CLASS##Entry{TYPE, []() { return std::make_unique<tomato::CLASS>(); }}; }

#define REGISTER_CONTROLLER_SYSTEM(CLASS)\
namespace { static tomato::SystemRegistryEntry CLASS##Entry{[](std::vector<tomato::CharacterInputHistory>& input) { return std::make_unique<tomato::CLASS>(input); }}; }

#endif //TOMATO_SYSTEMREGISTRY_H
