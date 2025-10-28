#include "TestState.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/Rigidbody.h"
#include "tomato/ecs/components/Movement.h"
#include "tomato/ecs/components/Sprite.h"

void TestState::Init(tomato::World& world)
{
    const auto entity = world.Create();

    world.Emplace<tomato::PositionComponent>(entity);
    world.Emplace<tomato::RotationComponent>(entity);
    world.Emplace<tomato::ScaleComponent>(entity, (glm::vec3{ 190.f, 191.f, 1.f }));
    world.Emplace<tomato::WorldMatrixComponent>(entity);

    world.Emplace<tomato::SpeedComponent>(entity, (float)50.f);

    world.Emplace<tomato::InputChannelComponent>(entity, (uint8_t)0);
    world.Emplace<tomato::MovementComponent>(entity);

    world.Emplace<tomato::SpriteComponent>(entity, (uint16_t)0, (uint16_t)0);
}

void TestState::Exit() {}