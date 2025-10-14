#include "tomato/ecs/SystemManager.h"
#include "tomato/ecs/systems/System.h"
#include "tomato/ecs/World.h"
#include "tomato/SimulationContext.h"
#include "tomato/services/CharacterInputHistory.h"

#include "tomato/ecs/systems/TransformSystem.h"
#include "tomato/ecs/systems/KinematicMovementSystem.h"

namespace tomato
{
    SystemManager::SystemManager(std::vector<CharacterInputHistory>& inputHistory)
    {
        // !!! TEST !!! 수정 필요 !!!
        controllers_.emplace_back(std::make_unique<KinematicMovementSystem>(inputHistory));
        collisions_.emplace_back(std::make_unique<TransformSystem>());
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