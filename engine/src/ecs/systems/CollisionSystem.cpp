#include "tomato/ecs/systems/CollisionSystem.h"
#include "tomato/Engine.h"
#include "tomato/tomato_sim.h"
#include "tomato/Logger.h"
#include "tomato/ecs/components/Collision.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/utils/BitmaskOperators.h"
#include <list>
#include <limits>
#include <vector>

#include "tomato/RegistryEntry.h"
REGISTER_SYSTEM(tomato::SystemPhase::COLLISION, CollisionSystem);

namespace tomato
{
    void CollisionSystem::Update(Engine& engine, const SimContext& ctx)
    {
        auto& reg = engine.GetWorld().GetRegistry();
        auto group = reg.group<ColliderComponent>();
        for (auto [e, col] : group.each())
        {
            if (col.aabbDirty)
                SetAABB(reg, e, col);
        }

        group.sort<ColliderComponent>([](const auto& l, const auto& r)
                                  { return l.min.x < r.min.x; });

        std::list<entt::entity> active;
        float activeMaxX = std::numeric_limits<float>::lowest();

        for (auto [e, col] : group.each())
        {
            if (activeMaxX < col.min.x)
            {
                active.clear();

                active.push_back(e);
                activeMaxX = col.max.x;
            }
            else
            {
                for (auto it = active.begin(); it != active.end();)
                {
                    auto& colAct = reg.get<ColliderComponent>(*it);

                    if (colAct.max.x < col.min.x)
                    {
                        active.erase(it++);
                        continue;
                    }

                    if (!layerMatrix_.CanCollide(col.layer, colAct.layer))
                    {
                        ++it;
                        continue;
                    }

                    if (colAct.max.y < col.min.y || col.max.y < colAct.min.y)
                    {
                        ++it;
                        continue;
                    }
                    if (colAct.max.z < col.min.z || col.max.z < colAct.min.z)
                    {
                        ++it;
                        continue;
                    }

                    TMT_INFO << "AABB Collision";

                    // narrow-phase


                    ++it;
                }

                active.push_back(e);
                activeMaxX = std::max(activeMaxX, col.max.x);
            }
        }
    }

    void CollisionSystem::SetAABB(Registry& reg, const Entity e, ColliderComponent& col)
    {
        auto& pos = reg.get<PositionComponent>(e);

        switch (col.type)
        {
            case ColliderType::Sphere:
            {
                glm::vec3 radius{col.halfExtents.x};

                col.max = pos.position + radius;
                col.min = pos.position - radius;
            }
                break;
            default:
            {
                auto& rot = reg.get<RotationComponent>(e);
                auto R = glm::toMat4(glm::quat(glm::radians(rot.eulerDegree)));

                glm::vec3 aabbHalfExtents = glm::vec3
                {
                    glm::abs(R[0][0]) * col.halfExtents.x + glm::abs(R[1][0]) * col.halfExtents.y + glm::abs(R[2][0]) * col.halfExtents.z,
                    glm::abs(R[0][1]) * col.halfExtents.x + glm::abs(R[1][1]) * col.halfExtents.y + glm::abs(R[2][1]) * col.halfExtents.z,
                    glm::abs(R[0][2]) * col.halfExtents.x + glm::abs(R[1][2]) * col.halfExtents.y + glm::abs(R[2][2]) * col.halfExtents.z
                };

                col.max = pos.position + col.position + aabbHalfExtents;
                col.min = pos.position + col.position - aabbHalfExtents;
            }
                break;
        }
    }

    bool CollisionSystem::GJK(Engine& engine, const SimContext& ctx)
    {
        std::vector<glm::vec3> simplex;
        simplex.reserve(4);
    }
}