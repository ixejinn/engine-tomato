#include "tomato/ecs/systems/CollisionSystem.h"
#include "tomato/Engine.h"
#include "tomato/tomato_sim.h"
#include "tomato/Logger.h"
#include "tomato/ecs/components/Collision.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/utils/BitmaskOperators.h"
#include <list>
#include <limits>

#include "tomato/RegistryEntry.h"
REGISTER_SYSTEM(tomato::SystemPhase::COLLISION, CollisionSystem);

namespace tomato
{
    void CollisionSystem::Update(Engine& engine, const SimContext& ctx)
    {
        auto& reg = engine.GetWorld().GetRegistry();
        auto group = reg.group<AABBComponent, ColliderComponent>();
        for (auto [e, aabb, col] : group.each())
        {
            if (aabb.isDirty)
                SetAABB(reg, e, aabb);
        }

        group.sort<AABBComponent>([](const auto& l, const auto& r)
                                  { return l.min.x < r.min.x; });

        std::list<entt::entity> active;
        float activeMaxX = std::numeric_limits<float>::lowest();

        for (auto [e, aabb, col] : group.each())
        {
            if (activeMaxX < aabb.min.x)
            {
                active.clear();

                active.push_back(e);
                activeMaxX = aabb.max.x;
            }
            else
            {
                activeMaxX = std::max(activeMaxX, aabb.max.x);
                for (auto it = active.begin(); it != active.end();)
                {
                    auto& aabbAct = reg.get<AABBComponent>(*it);
                    auto& colAct = reg.get<ColliderComponent>(*it);

                    if (aabbAct.max.x < aabb.min.x)
                    {
                        active.erase(it++);
                        continue;
                    }

                    if (!layerMatrix_.CanCollide(col.layer, colAct.layer))
                    {
                        ++it;
                        continue;
                    }

                    if (aabbAct.max.y < aabb.min.y || aabb.max.y < aabbAct.min.y)
                    {
                        ++it;
                        continue;
                    }
                    if (aabbAct.max.z < aabb.min.z || aabb.max.z < aabbAct.min.z)
                    {
                        ++it;
                        continue;
                    }

                    TMT_INFO << "AABB Collision";

                    // narrow-phase
                    const ColliderType type = col.type | colAct.type;
                    if (type == (ColliderType::AABB | ColliderType::OBB))
                        ;
                    else if (type == (ColliderType::AABB | ColliderType::Sphere))
                        ;
                    else if (type == (ColliderType::AABB | ColliderType::Cylinder))
                        ;
                    else if (type == ColliderType::OBB)
                        ;
                    else if (type == (ColliderType::OBB | ColliderType::Sphere))
                        ;
                    else if (type == (ColliderType::OBB | ColliderType::Cylinder))
                        ;
                    else if (type == ColliderType::Sphere)
                        ;
                    else if (type == (ColliderType::Sphere | ColliderType::Cylinder))
                        ;
                    else          // ColliderType::Cylinder
                        ;


                    ++it;
                }

                active.push_back(e);
                activeMaxX = std::max(activeMaxX, aabb.max.x);
            }
        }
    }

    void CollisionSystem::SetAABB(Registry& reg, Entity e, AABBComponent& aabb)
    {
        auto& col = reg.get<ColliderComponent>(e);
        auto& pos = reg.get<PositionComponent>(e);

        switch (col.type)
        {
            // case ColliderType::Plain:
            case ColliderType::OBB:
            {
                auto& rot = reg.get<RotationComponent>(e);
                auto R = glm::toMat4(glm::quat(glm::radians(rot.eulerDegree)));

                glm::vec3 rotatedHalfScale = glm::vec3
                {
                    glm::abs(R[0][0]) * col.halfScale.x + glm::abs(R[1][0]) * col.halfScale.y + glm::abs(R[2][0]) * col.halfScale.z,
                    glm::abs(R[0][1]) * col.halfScale.x + glm::abs(R[1][1]) * col.halfScale.y + glm::abs(R[2][1]) * col.halfScale.z,
                    glm::abs(R[0][2]) * col.halfScale.x + glm::abs(R[1][2]) * col.halfScale.y + glm::abs(R[2][2]) * col.halfScale.z
                };

                aabb.max = pos.position + rotatedHalfScale;
                aabb.min = pos.position - rotatedHalfScale;
            }
                break;
            // case ColliderType::Circle:
            case ColliderType::Sphere:
            {
                glm::vec3 radius{col.halfScale.x};

                aabb.max = pos.position + radius;
                aabb.min = pos.position - radius;
            }
                break;
            case ColliderType::Cylinder:
            {
                glm::vec3 offset{col.halfScale.x, col.halfScale.y, col.halfScale.x};

                aabb.max = pos.position + offset;
                aabb.min = pos.position - offset;
            }
                break;
            case ColliderType::AABB:
            default:
                aabb.max = pos.position + col.halfScale;
                aabb.min = pos.position - col.halfScale;
                break;
        }
    }
}