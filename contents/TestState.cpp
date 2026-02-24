#include "TestState.h"
#include "tomato/tomato.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/Rigidbody.h"
#include "tomato/ecs/components/Movement.h"
#include "tomato/ecs/components/Render.h"
#include "tomato/resource/AssetRegistry.h"
#include "tomato/resource/render/Mesh.h"
#include "tomato/resource/render/Texture.h"
#include "tomato/resource/render/Shader.h"
#include <iostream>

void TestState::Init(tomato::World& world)
{
    tmt::Texture::Create("assets/WATER_GAME_LOGO.png");

    tmt::Registry& registry = world.GetRegistry();

    const auto me = world.CreateEntity();

    registry.emplace<tomato::PositionComponent>(me);
    registry.emplace<tomato::RotationComponent>(me);
    registry.emplace<tomato::ScaleComponent>(me, (glm::vec3{1.f, 1.f, 1.f }));
    registry.emplace<tomato::WorldMatrixComponent>(me);

    registry.emplace<tomato::SpeedComponent>(me, (float)1.f);

    registry.emplace<tomato::InputChannelComponent>(me, (uint8_t)0);
    registry.emplace<tomato::JumpComponent>(me);

    registry.emplace<tomato::RenderComponent>(me,
                                              tmt::Vector4(1.f, 1.f, 1.f, 1.f),
                                              tmt::GetAssetID(tmt::Mesh::GetName(tmt::Mesh::PrimitiveType::PLAIN)),
                                              tmt::GetAssetID(tmt::Shader::PrimitiveName),
                                              tmt::GetAssetID("assets/WATER_GAME_LOGO.png"));

    const auto other = world.CreateEntity();

    registry.emplace<tomato::PositionComponent>(other);
    registry.emplace<tomato::RotationComponent>(other);
    registry.emplace<tomato::ScaleComponent>(other, (glm::vec3{1.f, 1.f, 1.f }));
    registry.emplace<tomato::WorldMatrixComponent>(other);

    registry.emplace<tomato::SpeedComponent>(other, (float)1.f);

    registry.emplace<tomato::InputChannelComponent>(other, (uint8_t)1);
    registry.emplace<tomato::JumpComponent>(other);

    registry.emplace<tomato::RenderComponent>(other,
                                              tmt::Vector4(1.f, 1.f, 0.f, 1.f),
                                              tmt::GetAssetID(tmt::Mesh::GetName(tmt::Mesh::PrimitiveType::PLAIN)),
                                              tmt::GetAssetID(tmt::Shader::PrimitiveName),
                                              tmt::GetAssetID(tmt::Texture::PrimitiveName));
}

void TestState::Exit() {}