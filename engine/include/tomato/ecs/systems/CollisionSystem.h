#ifndef TOMATO_COLLISIONSYSTEM_H
#define TOMATO_COLLISIONSYSTEM_H

#include "tomato/ecs/systems/System.h"
#include "tomato/ecs/tomato_ecs.h"
#include "tomato/collision/CollisionLayerMatrix.h"

namespace tomato
{
    struct ColliderComponent;

    class CollisionSystem : public System
    {
    public:
        void Update(Engine& engine, const SimContext& ctx) override;

    private:
        static void SetAABB(Registry& reg, Entity e, ColliderComponent& col);
        static bool GJK(Registry& reg, Entity e1, Entity e2);

        CollisionLayerMatrix layerMatrix_;
    };
}

#endif //TOMATO_COLLISIONSYSTEM_H
