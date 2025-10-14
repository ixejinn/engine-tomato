#ifndef TOMATO_RIGIDBODY_H
#define TOMATO_RIGIDBODY_H

#include "glm/vec3.hpp"

namespace tomato
{
    struct MassComponent { float mass{1.f}; };
    struct DragComponent { float drag{0.f}; };

    struct SpeedComponent { float speed{1.f}; };

    struct GravityComponent { float gravity{10.f}; };
    struct VelocityComponent { glm::vec3 velocity; };
    struct ForceComponent { glm::vec3 force; };
}

#endif //TOMATO_RIGIDBODY_H
