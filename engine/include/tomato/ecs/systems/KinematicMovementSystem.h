#ifndef TOMATO_KINEMATICMOVEMENTSYSTEM_H
#define TOMATO_KINEMATICMOVEMENTSYSTEM_H

#include "System.h"
#include "tomato/services/CharacterInputHistory.h"  // std::vector<CharacterInputHistory>& inputHistory_

namespace tomato
{
    class KinematicMovementSystem : public System
    {
    public:
        KinematicMovementSystem(std::vector<CharacterInputHistory>& inputHistory)
                : inputHistory_(inputHistory) {}

        void Update(World& world, const SimContext& ctx) override;

    private:
        constexpr static uint8_t JUMP_COUNT_MAX{2};
        constexpr static float JUMP_SPEED{1.f};
        constexpr static float GRAVITY{-1.f};

        std::vector<CharacterInputHistory>& inputHistory_;
    };
}

#endif //TOMATO_KINEMATICMOVEMENTSYSTEM_H
