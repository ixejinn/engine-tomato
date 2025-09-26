#ifndef ORANGE_PLAYERCONTROLLER_H
#define ORANGE_PLAYERCONTROLLER_H

#endif //ORANGE_PLAYERCONTROLLER_H

#include <glm/vec2.hpp>

struct InputSnapshotComponent {
    glm::vec2 moveDirection{0.f, 0.f};
    bool isJumping{false};
};