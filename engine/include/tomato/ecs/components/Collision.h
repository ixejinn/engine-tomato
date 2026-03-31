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
        ColliderType type;
        Vector3 halfScale{0.5f};

        CollisionLayer layer = CollisionLayer::Default;

        bool isTrigger = false;
    };

    struct AABBComponent
    {
        Vector3 min;
        Vector3 max;
        bool isDirty = true;
    };

    using CollisionCallback = std::function<void(CollisionEvent&)>;
    struct OnCollisionComponent
    {
        CollisionCallback enter;
        // exit, stay
    };

    using TriggerCallback = std::function<void(TriggerEvent&)>;
    struct OnTriggerComponent
    {
        TriggerCallback enter;
    };
}

#endif //TOMATO_COLLISION_H
