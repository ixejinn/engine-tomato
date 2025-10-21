#include "tomato/ecs/systems/TransformSystem.h"
#include "glm/gtc/matrix_transform.hpp"
#include "tomato/ecs/World.h"
#include "tomato/ecs/components/Transform.h"

#include "tomato/ecs/SystemRegistry.h"
REGISTER_SYSTEM(tomato::SystemType::INTEGRATOR, TransformSystem)

namespace tomato
{
    void TransformSystem::Update(World& world, const SimContext& ctx)
    {
        auto view = world.View<PositionComponent, RotationComponent, ScaleComponent, WorldMatrixComponent>();

        for (auto [e, pos, rot, scl, mat] : view.each())
        {
            mat.matrix = glm::translate(glm::mat4(1.f), pos.position);
            mat.matrix = glm::rotate(mat.matrix, glm::radians(rot.yaw), glm::vec3(0.f, 0.f, 1.f));
            mat.matrix = glm::scale(mat.matrix, scl.scale);
        }
    }
}