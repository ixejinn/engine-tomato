#include "tomato/ecs/systems/KinematicMovementSystem.h"
#include "tomato/Engine.h"
#include "tomato/SimulationContext.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/Rigidbody.h"
#include "tomato/ecs/components/Movement.h"

#include "tomato/ecs/SystemRegistry.h"
REGISTER_SYSTEM(tomato::SystemType::CONTROLLER, KinematicMovementSystem);

namespace tomato
{
    void KinematicMovementSystem::Update(Engine& engine, const SimContext& ctx)
    {
        auto view = engine.GetWorld().View<PositionComponent, SpeedComponent, InputChannelComponent, MovementComponent>();
        auto inputTimeline = engine.GetInputTimeline();

        for (auto [e, pos, speed, ch, move] : view.each())
        {
            const auto& inputRec = inputTimeline[ch.channel][ctx.tick];
            if (inputRec.tick != ctx.tick)
                continue;

            InputAction keypress{inputRec.keypress};
            InputAction keydown{inputRec.keydown};

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

            pos.position.x += dir.x * speed.speed * Engine::FIXED_DELTA_TIME;
            pos.position.y += dir.y * speed.speed * Engine::FIXED_DELTA_TIME;

            // !!! for 3D MOVEMENT !!!
            //pos.position.z += dir.y * speed.speed * Engine::FIXED_DELTA_TIME;

            // 점프 처리
            /* !!! for 3D MOVEMENT !!!
            if (HasAction(keydown, InputAction::JUMP) && move.jumpCount < JUMP_COUNT_MAX)
            {
                // 점프 시작
                move.jumpCount++;
                move.vy = std::max(move.vy, 0.f) + JUMP_SPEED;
            }

            if (move.jumpCount > 0)
            {
                // 점프 중
                move.vy += GRAVITY * Engine::FIXED_DELTA_TIME;
                pos.position.y += move.vy * Engine::FIXED_DELTA_TIME;

                if (pos.position.y <= 0)
                {
                    // 점프 종료
                    move.jumpCount = 0;
                    pos.position.y = 0;
                    move.vy = 0.f;
                }
            }
            !!! for 3D MOVEMENT !!! */
        }
    }
}