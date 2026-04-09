#include "tomato/ecs/systems/KinematicMovementSystem.h"
#include "tomato/Engine.h"
#include "tomato/tomato_sim.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/Rigidbody.h"
#include "tomato/ecs/components/Movement.h"
#include "tomato/utils/BitmaskOperators.h"

#include "tomato/RegistryEntry.h"
REGISTER_SYSTEM(tomato::SystemPhase::CONTROLLER, KinematicMovementSystem);

namespace tomato
{
    void KinematicMovementSystem::Update(Engine& engine, const SimContext& ctx)
    {
        auto view = engine.GetWorld().GetRegistry().view<TransformComponent, SpeedComponent, InputChannelComponent, JumpComponent>();
        auto inputTimeline = engine.GetInputTimeline();

        for (auto [e, trf, speed, ch, move] : view.each())
        {
            auto pos = trf.GetPosition();
            const auto& inputRec = inputTimeline[ch.channel][ctx.tick];
            if (inputRec.tick != ctx.tick)
                continue;

            InputIntent keypress{inputRec.held};
            InputIntent keydown{inputRec.down};

            // Move
            int x = 0, y = 0;
            if (HasFlag(keypress, InputIntent::UP))
                y++;
            if (HasFlag(keypress, InputIntent::DOWN))
                y--;
            if (HasFlag(keypress, InputIntent::LEFT))
                x--;
            if (HasFlag(keypress, InputIntent::RIGHT))
                x++;
            glm::vec2 dir = glm::vec2{x, -y};
            if (glm::length(dir) > 1)
                dir = glm::normalize(dir);

            pos.x += dir.x * speed.speed * Engine::FIXED_DELTA_TIME;
            // !!! for 2D MOVEMENT !!!
            //pos.position.y += dir.y * speed.speed * Engine::FIXED_DELTA_TIME;

            // !!! for 3D MOVEMENT !!!
            pos.z += dir.y * speed.speed * Engine::FIXED_DELTA_TIME;

            // Jump
            if (HasFlag(keydown, InputIntent::JUMP) && move.cnt < JUMP_COUNT_MAX)
            {
                // Start jump
                move.cnt++;
                move.vy = std::max(move.vy, 0.f) + JUMP_SPEED;
            }

            if (move.cnt > 0)
            {
                // Jumping
                move.vy += GRAVITY * Engine::FIXED_DELTA_TIME;
                pos.y += move.vy * Engine::FIXED_DELTA_TIME;

                if (pos.y <= 0)
                {
                    // End jump
                    move.cnt = 0;
                    pos.y = 0;
                    move.vy = 0.f;
                }
            }

            trf.SetPosition(pos);
        }
    }
}