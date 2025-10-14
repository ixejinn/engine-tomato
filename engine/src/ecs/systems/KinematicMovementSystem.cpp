#include "tomato/ecs/systems/KinematicMovementSystem.h"
#include "tomato/SimulationContext.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/Rigidbody.h"
#include "tomato/ecs/components/Movement.h"

#include <iostream>

namespace tomato
{
    void KinematicMovementSystem::Update(World& world, const SimContext& ctx)
    {
        auto view = world.View<PositionComponent, SpeedComponent, InputChannelComponent, MovementComponent>();

        for (auto [e, pos, speed, ch, move] : view.each())
        {
            InputAction keypress{inputHistory_[ch.channel][ctx.tick].keypress};
            InputAction keydown{inputHistory_[ch.channel][ctx.tick].keydown};

            // 이동 처리
            int x = 0, y = 0;
            if (HasAction(keypress, InputAction::UP))
                y++;
            if (HasAction(keypress, InputAction::DOWN))
                y--;
            if (HasAction(keypress, InputAction::LEFT))
                x--;
            if (HasAction(keypress, InputAction::RIGHT))
                x++;
            glm::vec2 dir = glm::vec2{x, y};
            if (glm::length(dir) > 1)
                dir = glm::normalize(dir);

            pos.position.x += dir.x * speed.speed * ctx.dt;
            pos.position.z += dir.y * speed.speed * ctx.dt;

            // 점프 처리
            if (HasAction(keydown, InputAction::JUMP) && move.jumpCount < JUMP_COUNT_MAX)
            {
                // 점프 시작
                move.jumpCount++;
                move.vy = std::max(move.vy, 0.f) + JUMP_SPEED;
            }

            if (move.jumpCount > 0)
            {
                // 점프 중
                move.vy += GRAVITY * ctx.dt;
                pos.position.y += move.vy * ctx.dt;

                if (pos.position.y <= 0)
                {
                    // 점프 종료
                    move.jumpCount = 0;
                    pos.position.y = 0;
                    move.vy = 0.f;
                }
            }

            //std::cout << pos.position.x << ", " << pos.position.z << ", " << pos.position.y << std::endl;
        }
    }
}