#include "tomato/ecs/systems/TransformSystem.h"
#include "tomato/Engine.h"
#include "tomato/tomato_sim.h"
#include "tomato/ecs/components/Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "tomato/RegistryEntry.h"
REGISTER_SYSTEM(tomato::SystemPhase::INTEGRATOR, TransformSystem)

namespace tomato
{
    void TransformSystem::Update(Engine& engine, const SimContext& ctx)
    {
        auto view = engine.GetWorld().GetRegistry().view<TransformComponent>();

        for (auto [e, trf] : view.each())
        {
            // Scale → Rotate → Translate
            auto T = glm::translate(glm::mat4(1.f), trf.position);
            if (trf.rotDirty)
            {
                trf.rotation = glm::quat(glm::radians(trf.eulerDegree));
                trf.rotDirty = false;
            }
            auto R = glm::toMat4(trf.rotation);
            auto S = glm::scale(glm::mat4(1.f), trf.scale);

            trf.transformMatrix = T * R * S;
        }
    }
}