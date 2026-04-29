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
        auto& r = engine.GetWorld().GetRegistry();
        auto view = r.view<PositionComponent, RotationComponent, ScaleComponent, WorldMatrixComponent>();

        for (auto [e, pos, rot, scl, mat] : view.each())
        {
            // Scale → Rotate → Translate
            auto T = glm::translate(glm::mat4(1.f), pos.position);
            auto R = glm::toMat4(glm::quat(glm::radians(rot.eulerDegree)));
            auto S = glm::scale(glm::mat4(1.f), scl.scale);

            mat.matrix = T * R * S;
        }
#if 1
        auto& uiCtx = r.ctx().get<UIContext>();
        for (auto e : uiCtx.drawList)
        {
            auto& hierarchy = r.get<HierarchyComponent>(e);
            auto& rect = r.get<RectTransformComponent>(e);
            auto& ui = r.get<UIComponent>(e);

            // Scale → Rotate → Translate
            auto T = glm::translate(glm::mat4(1.f), rect.position);
            auto R = glm::toMat4(glm::quat(glm::radians(rect.rotation)));
            auto S = glm::scale(glm::mat4(1.f), rect.scale);

            rect.local_matrix = T * R * S;

            if (ui.canvas == e)
                rect.world_matrix = rect.local_matrix;

            else if (hierarchy.parent != entt::null)
            {
                auto& parentRect = r.get<tomato::RectTransformComponent>(hierarchy.parent);

                //std::cout << "child rect " << ui.type << " pos(" << rect.position.x << ", " << rect.position.y << ")\n";
                rect.world_matrix = parentRect.world_matrix * rect.local_matrix;
            }
            else
                rect.world_matrix = rect.local_matrix;

            glm::mat4 renderM = rect.world_matrix;
            if (ui.type != 2)
            {
                renderM = glm::scale(renderM, glm::vec3(rect.computedSize, 1.0f));
                renderM = glm::translate(renderM, glm::vec3(-rect.pivot, 0.f));
            }
            rect.model_matrix = renderM;
        }

#else 0
        //For UI
        auto viewUI = engine.GetWorld().GetRegistry().view<HierarchyComponent ,RectTransformComponent, UIComponent>();

        for (auto [e, hierarchy, rect, ui] : viewUI.each())
        {
            // Scale → Rotate → Translate
            auto T = glm::translate(glm::mat4(1.f), rect.position);
            auto R = glm::toMat4(glm::quat(glm::radians(rect.rotation)));
            auto S = glm::scale(glm::mat4(1.f), rect.scale);
            
            rect.local_matrix = T * R * S;
            
            if (ui.canvas == e)
                rect.world_matrix = rect.local_matrix;

            else if(hierarchy.parent != entt::null)
            {
                auto& parentRect = engine.GetWorld().GetRegistry()
                    .get<tomato::RectTransformComponent>(hierarchy.parent);
                //if (parentRect.world_matrix != glm::mat4(1.f)) // @TODO : parent world matrix first (sort)
                //{
                    //std::cout << "child rect pos(" << rect.position.x << ", " << rect.position.y << ")\n";
                    rect.world_matrix = parentRect.world_matrix * rect.local_matrix;
                //}
            }
            else
                rect.world_matrix = rect.local_matrix;

            glm::mat4 renderM = rect.world_matrix;
            renderM = glm::scale(renderM, glm::vec3(rect.computedSize, 1.0f));
            renderM = glm::translate(renderM, glm::vec3(-rect.pivot, 0.f));

            rect.model_matrix = renderM;
        }
#endif
    }

    void TransformSystem::UpdateScreenUI(Engine& engine)
    {
        auto viewUI = engine.GetWorld().GetRegistry().view<HierarchyComponent, RectTransformComponent, UIComponent>();

        for (auto [e, hierarchy, rect, ui] : viewUI.each())
        {
            // Scale → Rotate → Translate
            auto T = glm::translate(glm::mat4(1.f), rect.position);
            auto R = glm::toMat4(glm::quat(glm::radians(rect.rotation)));
            auto S = glm::scale(glm::mat4(1.f), rect.scale);

            rect.local_matrix = T * R * S;

            if (ui.canvas == e)
                rect.world_matrix = rect.local_matrix;

            else if (hierarchy.parent != entt::null)
            {
                auto& parentRect = engine.GetWorld().GetRegistry()
                    .get<tomato::RectTransformComponent>(hierarchy.parent);
                //if (parentRect.world_matrix != glm::mat4(1.f)) // @TODO : parent world matrix first (sort)
                //{
                    //std::cout << "child rect pos(" << rect.position.x << ", " << rect.position.y << ")\n";
                rect.world_matrix = parentRect.world_matrix * rect.local_matrix;
                //}
            }
            else
                rect.world_matrix = rect.local_matrix;

            glm::mat4 renderM = rect.world_matrix;
            renderM = glm::scale(renderM, glm::vec3(rect.computedSize, 1.0f));
            renderM = glm::translate(renderM, glm::vec3(-rect.pivot, 0.f));

            rect.model_matrix = renderM;
        }
    }
}