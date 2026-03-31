#ifndef TOMATO_COLLISIONEVENT_H
#define TOMATO_COLLISIONEVENT_H

#include "tomato/ecs/tomato_ecs.h"

namespace tomato
{
    struct CollisionEvent
    {
        Entity e1, e2;
    };

    struct TriggerEvent
    {
        Entity e1, e2;
    };
}

#endif //TOMATO_COLLISIONEVENT_H
