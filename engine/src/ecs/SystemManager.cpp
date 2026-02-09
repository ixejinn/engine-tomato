#include "tomato/ecs/SystemManager.h"
#include "tomato/ecs/SystemRegistry.h"
#include "tomato/Engine.h"
#include "tomato/ecs/systems/System.h"
#include "tomato/ecs/systems/RenderSystem.h"
#include "tomato/ecs/World.h"
#include "tomato/tomato_sim.h"

namespace tomato
{
    SystemManager::SystemManager()
    {
        auto registry = SystemRegistry::GetInstance();

        for (SystemPhase phase : simulationOrder_)
        {
            for (const auto& factory : registry.GetFactory(phase))
                systems_[phase].emplace_back(factory());
        }

        for (SystemPhase phase : renderOrder_)
        {
            for (const auto& factory : registry.GetFactory(phase))
                systems_[phase].emplace_back(factory());
        }
    }

    SystemManager::~SystemManager() = default;

    void SystemManager::Simulate(Engine& engine, const SimContext& ctx)
    {
        for (SystemPhase phase : simulationOrder_)
        {
            for (auto& system : systems_[phase])
                system->Update(engine, ctx);
        }
    }

    void SystemManager::Render(const Engine& engine, const SimContext& ctx)
    {
        // TODO: 나중에 빼기!!!!!
        glClear(GL_COLOR_BUFFER_BIT);

        for (SystemPhase phase : renderOrder_)
        {
            for (auto& system : systems_[phase])
                system->Update(engine, ctx);
        }
    }
}