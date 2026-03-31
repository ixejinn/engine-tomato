#ifndef TOMATO_COLLISIONCONSTANTS_H
#define TOMATO_COLLISIONCONSTANTS_H

#include <cstdint>

namespace tomato
{
    enum class CollisionLayer : uint32_t
    {
        Default = 1 << 0,
        COUNT
    };

    enum class ColliderType
    {
        Plain,
        Circle,
        AABB,
        OBB,
        Sphere,
        Cylinder
    };
}

#endif //TOMATO_COLLISIONCONSTANTS_H
