#include "tomato/ecs/SystemRegistry.h"
#include "tomato/ecs/systems/System.h"
#include "tomato/services/CharacterInputHistory.h"

namespace tomato
{
    SystemRegistry::~SystemRegistry() = default;

    void SystemRegistry::RegisterSystemFactory(const SystemType type, Factory&& factory)
    {
        switch (type)
        {
            case CONTROLLER:
                controllers_.push_back(std::move(factory));
                break;

            case INTEGRATOR:
                integrators_.push_back(std::move(factory));
                break;

            case COLLISION:
                collisions_.push_back(std::move(factory));
                break;

            case GAMERULE:
                gameRules_.push_back(std::move(factory));
                break;

            case SPAWNER:
                spawners_.push_back(std::move(factory));
                break;

            case RENDER:
                renderers_.push_back(std::move(factory));
                break;

            case CAMERA:
                camera_.push_back(std::move(factory));
                break;
        }
    }

    const std::vector<Factory>& SystemRegistry::GetFactory(const SystemType type)
    {
        switch (type)
        {
            case CONTROLLER:
                return controllers_;

            case INTEGRATOR:
                return integrators_;

            case COLLISION:
                return collisions_;

            case GAMERULE:
                return gameRules_;

            case SPAWNER:
                return spawners_;

            case RENDER:
                return renderers_;
            
            case CAMERA:
                return camera_;
        }
        // !!! ERROR !!!
        return integrators_;
    }
}