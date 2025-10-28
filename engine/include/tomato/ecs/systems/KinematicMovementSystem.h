#ifndef TOMATO_KINEMATICMOVEMENTSYSTEM_H
#define TOMATO_KINEMATICMOVEMENTSYSTEM_H

#include "System.h"
#include <cstdint>

namespace tomato
{
    class Engine;

    class KinematicMovementSystem : public System
    {
    public:
        void Update(Engine& engine, const SimContext& ctx) override;

    private:
        constexpr static uint8_t JUMP_COUNT_MAX{2};
        constexpr static float JUMP_SPEED{1.f};
        constexpr static float GRAVITY{-1.f};
    };
}

#endif //TOMATO_KINEMATICMOVEMENTSYSTEM_H
