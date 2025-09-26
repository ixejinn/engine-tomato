#ifndef TOMATO_RIGIDBODY_H
#define TOMATO_RIGIDBODY_H

#endif //TOMATO_RIGIDBODY_H

#include <glm/vec3.hpp>

struct MassComponent { float mass{1.f}; };
struct DragComponent { float drag{0.f}; };

struct GravityComponent { float gravity{10.f}; };
struct VelocityComponent { glm::vec3 velocity; };
struct ForceComponent { glm::vec3 force; };