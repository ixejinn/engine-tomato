#ifndef ORANGE_TRANSFORM_H
#define ORANGE_TRANSFORM_H

#endif //ORANGE_TRANSFORM_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct PositionComponent { glm::vec3 position; };
struct RotationComponent { float yaw; };
struct ScaleComponent { glm::vec2 scale{1.f, 1.f}; };

// Local to World
struct WorldMatrixComponent { glm::mat4 matrix; };