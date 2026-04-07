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
        Cube,
        Sphere,
        Capsule,
        COUNT
    };
}

#endif //TOMATO_COLLISIONCONSTANTS_H
