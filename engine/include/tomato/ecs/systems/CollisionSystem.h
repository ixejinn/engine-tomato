#ifndef TOMATO_COLLISIONSYSTEM_H
#define TOMATO_COLLISIONSYSTEM_H

#include "tomato/ecs/systems/System.h"
#include "tomato/ecs/tomato_ecs.h"
#include "tomato/collision/CollisionLayerMatrix.h"
#include "tomato/collision/CollisionConstants.h"
#include "tomato/containers/EnumArray.h"
#include "tomato/tomato_math.h"

namespace tomato
{
    struct ColliderComponent;
    struct TransformComponent;

    class CollisionSystem : public System
    {
    public:
        CollisionSystem();

        void Update(Engine& engine, const SimContext& ctx) override;

    private:
        static void SetAABB(ColliderComponent& col, const TransformComponent& trf);

        bool GJK(const ColliderComponent& col1, const TransformComponent& trf1,
                        const ColliderComponent& col2, const TransformComponent& trf2);
        Vector3 Support(const Vector3& worldDir, const ColliderComponent& col, const TransformComponent& trf);
        bool AddSimplexPoint(std::vector<glm::vec3>& simplex,
                                    const ColliderComponent& col1, const TransformComponent& trf1,
                                    const ColliderComponent& col2, const TransformComponent& trf2);

        static bool VoronoiRegion(std::vector<glm::vec3>& simplex);
        static Vector3 GetOrientedNormal(const Vector3& refP, const Vector3& p0, const Vector3& p1, const Vector3& p2);

        CollisionLayerMatrix layerMatrix_;

        using SupportFunc = std::function<Vector3(const Vector3& dir, const ColliderComponent& col)>;
        EnumArray<ColliderType, SupportFunc> supportFunctions_;
    };
}

#endif //TOMATO_COLLISIONSYSTEM_H
