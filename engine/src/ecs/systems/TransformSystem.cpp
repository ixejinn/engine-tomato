#include "tomato/ecs/systems/TransformSystem.h"
#include "tomato/Engine.h"
#include "tomato/tomato_sim.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/UI.h"
#include "tomato/Logger.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "tomato/RegistryEntry.h"
REGISTER_SYSTEM(tomato::SystemPhase::INTEGRATOR, TransformSystem)

namespace tomato
{
    void TransformSystem::Update(Engine& engine, const SimContext& ctx)
    {
        auto view = engine.GetWorld().GetRegistry().view<PositionComponent, RotationComponent, ScaleComponent, WorldMatrixComponent>();

        for (auto [e, pos, rot, scl, mat] : view.each())
        {
            // Scale → Rotate → Translate
            auto T = glm::translate(glm::mat4(1.f), pos.position);
            auto R = glm::toMat4(glm::quat(glm::radians(rot.eulerDegree)));
            auto S = glm::scale(glm::mat4(1.f), scl.scale);

            mat.matrix = T * R * S;
        }

        auto viewUI = engine.GetWorld().GetRegistry().view<RectTransformComponent, UIComponent>();

        for (auto [e, rect, ui] : viewUI.each())
        {
            // Scale → Rotate → Translate
            auto T = glm::translate(glm::mat4(1.f), rect.position);
            auto R = glm::toMat4(glm::quat(glm::radians(rect.rotation)));
            auto S = glm::scale(glm::mat4(1.f), rect.scale);

            rect.local_matrix = T * R * S;
            
            if (ui.canvas == e)
                rect.world_matrix = rect.local_matrix;
            
            //@TODO : calculate world matrix with parent world matrix
        }
    }
}