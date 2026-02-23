#ifndef TOMATO_TRANSFORM_H
#define TOMATO_TRANSFORM_H

#include "tomato/tomato_math.h"

namespace tomato
{
    struct PositionComponent { Vector3 position{ 0.f, 0.f, 0.f }; };
    struct RotationComponent { Vector3 rotation{ 0.f, 0.f, 0.f }; };
    struct ScaleComponent { Vector3 scale{1.f, 1.f, 1.f}; };

    // Local to World
    struct WorldMatrixComponent { Matrix matrix; };
}

#endif //TOMATO_TRANSFORM_H
