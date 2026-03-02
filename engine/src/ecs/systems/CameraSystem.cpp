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
        auto view = engine.GetWorld().GetRegistry().view<PositionComponent, RotationComponent, CameraComponent>();

        for (auto [e, pos, rot, cam] : view.each())
        {
////// 1. 방향 벡터 구하기
////// 쿼터니언 회전이 없을 때(Identity), f는 (0, 0, 1)이 아니라 (0, 0, -1)이어야 합니다. (앞을 봐야 하니까요)
            auto quaternion = glm::quat(glm::radians(rot.eulerDegree));
            glm::vec3 f = quaternion * glm::vec3(0, 0, 1);
            glm::vec3 r = quaternion * glm::vec3(1, 0, 0);
            glm::vec3 u = quaternion * glm::vec3(0, 1, 0);
////
////// 2. 뷰 행렬의 Z축은 "카메라의 뒤쪽"이어야 하므로 -f를 사용합니다.
            glm::vec3 b = -f;

            glm::mat4 viewMtx(1.0f);

// 회전 부분 (Transpose 적용)
            viewMtx[0][0] = r.x; viewMtx[1][0] = r.y; viewMtx[2][0] = r.z;
            viewMtx[0][1] = u.x; viewMtx[1][1] = u.y; viewMtx[2][1] = u.z;
            viewMtx[0][2] = b.x; viewMtx[1][2] = b.y; viewMtx[2][2] = b.z; // f가 아니라 b(-f)입니다!

// 이동 부분 (Translation Inverse)
            viewMtx[3][0] = -glm::dot(r, pos.position);
            viewMtx[3][1] = -glm::dot(u, pos.position);
            viewMtx[3][2] = -glm::dot(b, pos.position); // f가 아니라 b입니다!
            viewMtx[3][3] = 1.0f;

//            auto viewMtx = glm::mat4(1.f);
//            viewMtx = glm::translate(viewMtx, glm::vec3(0.f, 0.f, -10.f));

//            auto viewMtx = glm::lookAt(pos.position, pos.position + f, u);

            glm::mat4 projection{1.f};
            auto& window = engine.GetWindow();
            switch (cam.mode)
            {
                case PERSPECTIVE:
                    projection = glm::perspective(glm::radians(cam.degree), (float)window.GetWidth() / window.GetHeight(), cam.zNear, cam.zFar);
                    break;
                case ORTHOGONAL:
                    projection = glm::ortho(0.f, (float)window.GetWidth(), 0.f, (float)window.GetHeight(), cam.zNear, cam.zFar);
                    break;
            }

            cam.viewProjection = projection * viewMtx;
        }
    }
}