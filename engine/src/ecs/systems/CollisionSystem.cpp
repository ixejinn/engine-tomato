#include "tomato/ecs/systems/CollisionSystem.h"
#include "tomato/Engine.h"
#include "tomato/tomato_sim.h"
#include "tomato/Logger.h"
#include "tomato/ecs/components/Collision.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/utils/BitmaskOperators.h"
#include "tomato/collision/ColliderSupport.h"
#include <list>
#include <limits>
#include <vector>
#include <glm/glm.hpp>

#include "tomato/RegistryEntry.h"
REGISTER_SYSTEM(tomato::SystemPhase::COLLISION, CollisionSystem);

namespace tomato
{
    CollisionSystem::CollisionSystem()
    {
        supportFunctions_[ColliderType::Cube] = support::Cube;
        supportFunctions_[ColliderType::Sphere] = support::Sphere;
        supportFunctions_[ColliderType::Capsule] = support::Capsule;
    }

    void CollisionSystem::Update(Engine& engine, const SimContext& ctx)
    {
        auto& reg = engine.GetWorld().GetRegistry();
        auto group = reg.group<ColliderComponent>();
        for (auto [e, col] : group.each())
        {
            if (col.aabbDirty)
                SetAABB(col, reg.get<TransformComponent>(e));
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

                    // narrow-phase
                    if (GJK(col, reg.get<TransformComponent>(e), colAct, reg.get<TransformComponent>(*it)))
                        TMT_INFO << "Collision";

                    ++it;
                }

                active.push_back(e);
                activeMaxX = std::max(activeMaxX, col.max.x);
            }
        }
    }

    void CollisionSystem::SetAABB(ColliderComponent& col, const TransformComponent& trf)
    {
        const auto& pos = trf.GetPosition();

        switch (col.type)
        {
            case ColliderType::Sphere:
            {
                glm::vec3 radius{col.halfExtents.x};

                col.max = pos + radius;
                col.min = pos - radius;
            }
                break;
            default:
            {
                auto R = glm::toMat4(trf.GetQuaternion());

                glm::vec3 aabbHalfExtents = glm::vec3
                {
                    glm::abs(R[0][0]) * col.halfExtents.x + glm::abs(R[1][0]) * col.halfExtents.y + glm::abs(R[2][0]) * col.halfExtents.z,
                    glm::abs(R[0][1]) * col.halfExtents.x + glm::abs(R[1][1]) * col.halfExtents.y + glm::abs(R[2][1]) * col.halfExtents.z,
                    glm::abs(R[0][2]) * col.halfExtents.x + glm::abs(R[1][2]) * col.halfExtents.y + glm::abs(R[2][2]) * col.halfExtents.z
                };

                col.max = pos + col.position + aabbHalfExtents;
                col.min = pos + col.position - aabbHalfExtents;
            }
                break;
        }
    }

    bool CollisionSystem::GJK(const ColliderComponent& col1, const TransformComponent& trf1,
                              const ColliderComponent& col2, const TransformComponent& trf2)
    {
        std::vector<glm::vec3> simplex;
        simplex.reserve(4);

        const auto R1 = glm::toMat4(trf1.GetQuaternion());
        glm::vec3 wPosCol1 = trf1.GetTransformMatrix() * glm::vec4(col1.position, 1.f);

        const auto R2 = glm::toMat4(trf2.GetQuaternion());
        glm::vec3 wPosCol2 = trf2.GetTransformMatrix() * glm::vec4(col2.position, 1.f);

        simplex.push_back(Support(wPosCol2 - wPosCol1, col1, trf1) - Support(wPosCol1 - wPosCol2, col2, trf2));

        while (true)
        {
            if (!AddSimplexPoint(simplex, col1, trf1, col2, trf2))
                return false;   // 비충돌 종료

            if (VoronoiRegion(simplex))
                return true;    // 충돌 종료
        }

    }

    Vector3 CollisionSystem::Support(const Vector3& worldDir, const ColliderComponent& col, const TransformComponent& trf)
    {
        const auto R = glm::toMat4(trf.GetQuaternion());
        const glm::vec4 localDir = glm::transpose(R) * glm::vec4(worldDir, 0.f);

        const auto localSupportP = supportFunctions_[col.type](localDir, col);
        return trf.GetPosition() + glm::vec3(R * glm::vec4(col.position, 1.f)) + glm::vec3(R * glm::vec4(localSupportP, 1.f));
    }

    bool CollisionSystem::AddSimplexPoint(std::vector<glm::vec3> &simplex,
                                          const ColliderComponent& col1, const TransformComponent& trf1,
                                          const ColliderComponent& col2, const TransformComponent& trf2)
    {
        glm::vec3 dir;
        auto simplexSize = simplex.size();
        switch (simplexSize)
        {
            case 1:
                dir = -simplex[0];
                simplex.push_back(Support(dir, col1, trf1) - Support(-dir, col2, trf2));

                if (glm::dot(dir, simplex[1]) < 0)
                    return false;   // 심플렉스가 원점을 포함할 수 없음
            break;
            case 2:
            {
                const auto ab = simplex[1] - simplex[0];
                const auto ao = -simplex[0];
                dir = glm::cross(glm::cross(ab, ao), ab);
                simplex.push_back(Support(dir, col1, trf1) - Support(-dir, col2, trf2));

                if (glm::dot(dir, simplex[2]) < 0)
                    return false;
            }
            break;
            case 3:
                dir = GetOrientedNormal(Vector3{0.f}, simplex[0], simplex[1], simplex[2]);
                simplex.push_back(Support(dir, col1, trf1) - Support(-dir, col2, trf2));

                if (glm::dot(dir, simplex[3]) < 0)
                    return false;
            break;
            default:
                TMT_WARN << "Incorrect simplex points.";
        }

        return true;
    }

    bool CollisionSystem::VoronoiRegion(std::vector<glm::vec3>& simplex)
    {
        auto simplexSize = simplex.size();
        switch (simplexSize)
        {
            case 2:
            {
                const auto ao = -simplex[0];
                const auto ab = simplex[1] - simplex[0];

                const auto dotV = glm::dot(ao, ab);
                if (dotV < 0)
                    simplex.pop_back();
                else if (dotV > glm::dot(ab, ab))
                    simplex.erase(simplex.begin());

                return false;
            }
            break;

            case 3:
            {
                const auto ab = simplex[1] - simplex[0];
                const auto bc = simplex[2] - simplex[1];
                const auto ac = simplex[2] - simplex[0];
                const auto ao = -simplex[0];
                const auto bo = -simplex[1];

                const auto normal = glm::cross(ac, ab);

                if (glm::dot(ao, glm::cross(normal, ab)) > 0)
                {
                    simplex.pop_back();
                    return false;
                }

                if (glm::dot(bo, glm::cross(normal, bc)) > 0)
                {
                    simplex.erase(simplex.begin());
                    return false;
                }

                if (glm::dot(ao, glm::cross(normal, -ac)) > 0)
                {
                    simplex.erase(++simplex.begin());
                    return false;
                }

                // 2차원이면 return true;
                return false;
            }
                break;

            case 4:
            {
                const auto lo = -simplex[3];

                float x;
                x = glm::dot(-GetOrientedNormal(simplex[1], simplex[3], simplex[0], simplex[2]), lo);
                if (x > 0)
                {
                    simplex.erase(++simplex.begin());
                    return false;
                }
                if (glm::dot(-GetOrientedNormal(simplex[0], simplex[3], simplex[2], simplex[1]), lo) > 0)
                {
                    simplex.erase(simplex.begin());
                    return false;
                }
                if (glm::dot(-GetOrientedNormal(simplex[2], simplex[3], simplex[1], simplex[0]), lo) > 0)
                {
                    simplex.erase(simplex.begin() + 2);
                    return false;
                }
                return true;
            }
                break;

            default:
                TMT_WARN << "Incorrect simplex size.";
                return false;
        }
        return false;
    }

    Vector3 CollisionSystem::GetOrientedNormal(const Vector3& refP, const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
        const auto vec01 = p1 - p0;
        const auto vec02 = p2 - p0;
        auto normal = glm::cross(vec01, vec02);
        auto x = glm::dot(normal, (refP - p0));
        return (x > 0 ? normal : -normal);
    }
}