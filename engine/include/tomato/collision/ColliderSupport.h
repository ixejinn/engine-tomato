#ifndef TOMATO_COLLIDERSUPPORT_H
#define TOMATO_COLLIDERSUPPORT_H

#include "tomato/tomato_math.h"

namespace tomato
{
    struct ColliderComponent;

    namespace support
    {
        Vector3 Cube(const Vector3& dir, const ColliderComponent& col);
        Vector3 Sphere(const Vector3& dir, const ColliderComponent& col);
        Vector3 Capsule(const Vector3& dir, const ColliderComponent& col);
    }
}

#endif //TOMATO_COLLIDERSUPPORT_H