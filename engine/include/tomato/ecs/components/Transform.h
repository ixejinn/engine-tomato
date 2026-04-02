#ifndef TOMATO_TRANSFORM_H
#define TOMATO_TRANSFORM_H

#include "tomato/tomato_math.h"

namespace tomato
{
    struct PositionComponent { Vector3 position{ 0.f, 0.f, 0.f }; };

    struct RotationComponent {
        Vector3 eulerDegree;
        //Quaternion rotation;
    };

    struct ScaleComponent { Vector3 scale{1.f, 1.f, 1.f}; };

    struct TransformComponent
    {
        Vector3 position{ 0.f, 0.f, 0.f };
        Vector3 scale{ 1.f, 1.f, 1.f };
        Vector3 eulerDegree;

        Entity parent{ entt::null };
        std::vector<Entity> children;

        bool dirty{ true };
    };

    // Local to World
    struct WorldMatrixComponent { Matrix4 matrix; };
}

#endif //TOMATO_TRANSFORM_H
