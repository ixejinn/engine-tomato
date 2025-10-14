#ifndef TOMATO_MOVEMENT_H
#define TOMATO_MOVEMENT_H

namespace tomato
{
    struct InputChannelComponent { uint8_t channel; };

    struct MovementComponent
    {
        uint8_t jumpCount{0};
        float vy{0.f};
    };
}

#endif //TOMATO_MOVEMENT_H
