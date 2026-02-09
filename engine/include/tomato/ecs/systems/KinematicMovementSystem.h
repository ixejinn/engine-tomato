#ifndef TOMATO_KINEMATICMOVEMENTSYSTEM_H
#define TOMATO_KINEMATICMOVEMENTSYSTEM_H

#include "System.h"
#include <cstdint>

namespace tomato
{
    class Engine;

    /**
     * @brief Kinematic movement system that updates entity positions by direct coordinate calculation.
     *
     * 물리 시뮬레이션 없이 좌표 계산으로 이동을 처리하는 운동학적 이동 시스템.
     * 점프는 단순한 물리 시뮬레이션을 사용한다.
     *
     * @note 현재는 2D 이동만 가능
     */
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
