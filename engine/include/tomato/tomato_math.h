#ifndef TOMATO_TOMATO_MATH_H
#define TOMATO_TOMATO_MATH_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>

namespace tomato
{
    using Vector2 = glm::vec2;
    using Vector3 = glm::vec3;
    using Vector4 = glm::vec4;

    using Matrix3 = glm::mat3;
    using Matrix4 = glm::mat4;

    using Quaternion = glm::quat;
}

#endif //TOMATO_TOMATO_MATH_H
