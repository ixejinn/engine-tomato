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

    enum class ColliderType : uint8_t
    {
        AABB = 1 << 0,
        OBB = 1 << 1,
        Sphere = 1 << 2,
        Cylinder = 1 << 3,
    };

    enum class Collider2DType : uint8_t
    {
        AABB,
        OBB,
        Circle
    };
}

#endif //TOMATO_COLLISIONCONSTANTS_H
