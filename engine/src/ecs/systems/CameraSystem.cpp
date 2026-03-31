#include "tomato/ecs/systems/CameraSystem.h"
#include "tomato/Engine.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/Camera.h"
#include "tomato/services/WindowService.h"

#include <glm/gtc/matrix_transform.hpp>

#include "tomato/RegistryEntry.h"
REGISTER_SYSTEM(tomato::SystemPhase::CAMERA, CameraSystem)

namespace tomato
{
    void CameraSystem::Update(Engine& engine, const SimContext& ctx)
    {
        auto _view = engine.GetWorld().GetRegistry().view<PositionComponent, RotationComponent, CameraComponent>();

        for (auto [e, pos, rot, cam] : _view.each())
        {
            auto quaternion = glm::quat(glm::radians(rot.eulerDegree));
            glm::vec3 b = quaternion * glm::vec3(0, 0, 1);
            glm::vec3 r = quaternion * glm::vec3(1, 0, 0);
            glm::vec3 u = quaternion * glm::vec3(0, 1, 0);

            glm::mat4 view
            {
                r.x, u.x, b.x, 0,   // column 0
                r.y, u.y, b.y, 0,   // column 1
                r.z, u.z, b.z, 0,   // column 2
                -glm::dot(r, pos.position), -glm::dot(u, pos.position), -glm::dot(b, pos.position), 1
            };

            glm::mat4 projection{1.f};
            auto& window = engine.GetWindowService();
            switch (cam.mode)
            {
                case PERSPECTIVE:
                    projection = glm::perspective(glm::radians(cam.degree), (float)window.GetWidth() / window.GetHeight(), cam.zNear, cam.zFar);
                    break;
                case ORTHOGONAL:
                {
                    const float ratio = (float)window.GetWidth() / (float)window.GetHeight();
                    float width = ratio * 10;
                    projection = glm::ortho(-width, width, -10.f, 10.f,
                        cam.zNear, cam.zFar);
                }
                    break;
            }

            cam.viewProjection = projection * view;
        }
    }
}