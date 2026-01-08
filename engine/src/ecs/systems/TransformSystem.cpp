#include "tomato/ecs/systems/TransformSystem.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "tomato/Engine.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/components/Transform.h"

#include "tomato/ecs/SystemRegistry.h"
REGISTER_SYSTEM(tomato::SystemType::INTEGRATOR, TransformSystem)

namespace tomato
{
    void TransformSystem::Update(Engine& engine, const SimContext& ctx)
    {
        auto view = engine.GetWorld().GetRegistry().view<PositionComponent, RotationComponent, ScaleComponent, WorldMatrixComponent>();

        for (auto [e, pos, rot, scl, mat] : view.each())
        {
            mat.matrix = glm::translate(glm::mat4(1.f), pos.position);
            mat.matrix = glm::rotate(mat.matrix, glm::radians(rot.rotation.y), glm::vec3(0.f, 1.f, 0.f));
            mat.matrix = glm::rotate(mat.matrix, glm::radians(rot.rotation.x), glm::vec3(1.f, 0.f, 0.f));
            mat.matrix = glm::rotate(mat.matrix, glm::radians(rot.rotation.z), glm::vec3(0.f, 0.f, 1.f));
            mat.matrix = glm::scale(mat.matrix, scl.scale);
            //glm::mat4 tra_mtx = glm::translate(glm::mat4(1.0f), pos.position);
            //glm::mat4 rot_mtx = glm::rotate(glm::mat4(1.0f), glm::radians(rot.yaw), glm::vec3(0.0f, 0.0f, 1.0f));
            //glm::mat4 scl_mtx = glm::scale(glm::mat4(1.0f), scl.scale);
            //
            glm::mat4 h = glm::mat4(1.0f);
            h[0][0] = 2.0f / static_cast<float>(1600);
            h[1][1] = 2.0f / static_cast<float>(900);
            h[2][2] = 1.0f;
            h[3][3] = 1.0f;

            //glm::mat4 mdl_xform = tra_mtx * rot_mtx * scl_mtx;
            //mat.matrix = h * mdl_xform;
            mat.matrix = h * mat.matrix;
        }
    }
}