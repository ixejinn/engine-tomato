#ifndef TOMATO_TRANSFORM_H
#define TOMATO_TRANSFORM_H

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

namespace tomato
{
    struct PositionComponent { glm::vec3 position{ 0.f, 0.f, 0.f }; };
    struct RotationComponent { glm::vec3 rotation{ 0.f, 0.f, 0.f }; };
    struct ScaleComponent { glm::vec3 scale{1.f, 1.f, 1.f}; };

    // Local to World
    struct WorldMatrixComponent { glm::mat4 matrix; };
}

#endif //TOMATO_TRANSFORM_H
