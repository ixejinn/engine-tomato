#include "TestState.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/Rigidbody.h"
#include "tomato/ecs/components/Movement.h"

void TestState::Init(tomato::World& world)
{
    const auto entity = world.Create();

    world.Emplace<tomato::PositionComponent>(entity);
    world.Emplace<tomato::RotationComponent>(entity);
    world.Emplace<tomato::ScaleComponent>(entity);
    world.Emplace<tomato::WorldMatrixComponent>(entity);

    world.Emplace<tomato::SpeedComponent>(entity);

    world.Emplace<tomato::InputChannelComponent>(entity, (uint8_t)0);
    world.Emplace<tomato::MovementComponent>(entity);
}

void TestState::Exit() {}