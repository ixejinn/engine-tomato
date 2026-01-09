#include "TestState.h"
#include "tomato/tomato.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/Rigidbody.h"
#include "tomato/ecs/components/Movement.h"
#include "tomato/ecs/components/Sprite.h"
#include <iostream>

void TestState::Init(tomato::World& world)
{
    tmt::Registry& registry = world.GetRegistry();

    const auto me = world.CreateEntity();

    registry.emplace<tomato::PositionComponent>(me);
    registry.emplace<tomato::RotationComponent>(me);
    registry.emplace<tomato::ScaleComponent>(me, (glm::vec3{190.f, 191.f, 1.f }));
    registry.emplace<tomato::WorldMatrixComponent>(me);

    registry.emplace<tomato::SpeedComponent>(me, (float)50.f);

    registry.emplace<tomato::InputChannelComponent>(me, (uint8_t)0);
    registry.emplace<tomato::MovementComponent>(me);

    registry.emplace<tomato::SpriteComponent>(me, (uint16_t)0, (uint16_t)0);

    const auto other = world.CreateEntity();

    registry.emplace<tomato::PositionComponent>(other);
    registry.emplace<tomato::RotationComponent>(other);
    registry.emplace<tomato::ScaleComponent>(other, (glm::vec3{190.f, 191.f, 1.f }));
    registry.emplace<tomato::WorldMatrixComponent>(other);

    registry.emplace<tomato::SpeedComponent>(other, (float)50.f);

    registry.emplace<tomato::InputChannelComponent>(other, (uint8_t)1);
    registry.emplace<tomato::MovementComponent>(other);

    registry.emplace<tomato::SpriteComponent>(other, (uint16_t)0, (uint16_t)0);
}

void TestState::Exit() {}