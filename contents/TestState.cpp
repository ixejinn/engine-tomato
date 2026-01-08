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
    const auto entity = world.CreateEntity();
    tmt::Registry& registry = world.GetRegistry();

    registry.emplace<tomato::PositionComponent>(entity);
    registry.emplace<tomato::RotationComponent>(entity);
    registry.emplace<tomato::ScaleComponent>(entity, (glm::vec3{ 190.f, 191.f, 1.f }));
    registry.emplace<tomato::WorldMatrixComponent>(entity);

    registry.emplace<tomato::SpeedComponent>(entity, (float)50.f);

    registry.emplace<tomato::InputChannelComponent>(entity, (uint8_t)1);
    registry.emplace<tomato::MovementComponent>(entity);

    registry.emplace<tomato::SpriteComponent>(entity, (uint16_t)0, (uint16_t)0);
}

void TestState::Exit() {}