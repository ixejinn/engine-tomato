#include "tomato/ecs/SystemManager.h"
#include "tomato/ecs/SystemRegistry.h"
#include "tomato/ecs/systems/System.h"
#include "tomato/ecs/systems/RenderSystem.h"
#include "tomato/ecs/systems/CameraSystem.h"
#include "tomato/SimulationContext.h"

namespace tomato
{
    SystemManager::SystemManager()
    {
        for (const auto& factory : SystemRegistry::GetInstance().GetFactory(CONTROLLER))
            controllers_.emplace_back(factory());

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

        for (const auto& factory : SystemRegistry::GetInstance().GetFactory(CAMERA))
            camera_.emplace_back(factory());
    }

    SystemManager::~SystemManager() = default;

    void SystemManager::Simulate(Engine& engine, const SimContext& ctx)
    {
        // input
        for (auto& system : controllers_)
            system->Update(engine, ctx);

        // physics
        for (auto& system : integrators_)
            system->Update(engine, ctx);
        for (auto& system : collisions_)
            system->Update(engine, ctx);

        // game logic
        for (auto& system : gameRules_)
            system->Update(engine, ctx);

        // spawn
        for (auto& system : spawners_)
            system->Update(engine, ctx);

        // camera
        //for (auto& system : camera_)
        //    system->Update(engine, ctx);
    }

    void SystemManager::Render(const Engine& engine, const SimContext& ctx)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        for (const auto& system : renderers_)
            system->Update(engine, ctx);
    }
}