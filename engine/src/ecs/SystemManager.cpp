#include "tomato/ecs/SystemManager.h"
#include "tomato/ecs/SystemRegistry.h"
#include "tomato/ecs/systems/System.h"
#include "tomato/ecs/World.h"
#include "tomato/SimulationContext.h"
#include "tomato/services/CharacterInputHistory.h"

namespace tomato
{
    SystemManager::SystemManager(std::vector<CharacterInputHistory>& inputHistory)
    {
        for (const auto& factory : SystemRegistry::GetInstance().GetControllerFactory())
            controllers_.emplace_back(factory(inputHistory));

        for (const auto& factory : SystemRegistry::GetInstance().GetFactory(INTEGRATOR))
            integrators_.emplace_back(factory());
        for (const auto& factory : SystemRegistry::GetInstance().GetFactory(COLLISION))
            collisions_.emplace_back(factory());

        for (const auto& factory : SystemRegistry::GetInstance().GetFactory(GAMERULE))
            gameRules_.emplace_back(factory());

        for (const auto& factory : SystemRegistry::GetInstance().GetFactory(SPAWNER))
            spawners_.emplace_back(factory());

        for (const auto& factory : SystemRegistry::GetInstance().GetFactory(RENDER))
            renderers_.emplace_back(factory());
    }

    SystemManager::~SystemManager() = default;

    void SystemManager::Simulate(World& world, const SimContext& ctx)
    {
        // input
        for (auto& system : controllers_)
            system->Update(world, ctx);

        // physics
        for (auto& system : integrators_)
            system->Update(world, ctx);
        for (auto& system : collisions_)
            system->Update(world, ctx);

        // game logic
        for (auto& system : gameRules_)
            system->Update(world, ctx);

        // spawn
        for (auto& system : spawners_)
            system->Update(world, ctx);
    }

    void SystemManager::Render(const World& world, const SimContext& ctx)
    {
        for (const auto& system : renderers_)
            system->Update(world, ctx);
    }
}