#ifndef TOMATO_COLLISION_H
#define TOMATO_COLLISION_H

#include "tomato/collision/CollisionConstants.h"
#include "tomato/tomato_math.h"

namespace tomato
{
    struct CollisionEvent;
    struct TriggerEvent;

    struct ColliderComponent
    {
        ColliderComponent(ColliderType t, CollisionLayer l = CollisionLayer::Default, Vector3 pos = Vector3{0.f})
        : type(t), layer(l), position(pos) {}
        Vector3 position;
        Vector3 halfExtents{0.5f};
        Vector3 min, max;   // AABB for broad-phase collision detection

        CollisionLayer layer = CollisionLayer::Default;
        ColliderType type;

        bool isTrigger = false;
        bool aabbDirty = true;
    };

    // Callback function
    using CollisionCallback = std::function<void(const CollisionEvent&)>;
    struct OnCollisionComponent
    {
        CollisionCallback enter;
        // exit, stay
    };

    using TriggerCallback = std::function<void(const TriggerEvent&)>;
    struct OnTriggerComponent
    {
        TriggerCallback enter;
    };
}

#endif //TOMATO_COLLISION_H
