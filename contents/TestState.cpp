#include "TestState.h"
#include "tomato/tomato.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/Rigidbody.h"
#include "tomato/ecs/components/Movement.h"
#include "tomato/ecs/components/Render.h"
#include "tomato/ecs/components/Camera.h"
#include "tomato/ecs/components/Tags.h"
#include "tomato/ecs/components/Collision.h"
#include "tomato/ecs/components/Text.h"
#include "tomato/resource/AssetRegistry.h"
#include "tomato/resource/render/Mesh.h"
#include "tomato/resource/render/Texture.h"
#include "tomato/resource/render/Shader.h"
#include <iostream>

void TestState::Init(tomato::World& world)
{
    tmt::Texture::Create("assets/WATER_GAME_LOGO.png");

    tmt::Registry& registry = world.GetRegistry();

    const auto cam = world.CreateEntity();
    registry.emplace<tomato::TransformComponent>(cam,
//                                                 glm::vec3(0.f, 7.5f, 15.f), glm::vec3(-30.f, 0.f, 0.f));
                                                 glm::vec3(0.f, 1.f, 10.f), glm::vec3(0.f, 0.f, 0.f));
    registry.emplace<tomato::CameraComponent>(cam);
    registry.emplace<tomato::MainCameraTag>(cam);

    // registry.emplace<tomato::SpeedComponent>(cam, (float)2.f);
    // registry.emplace<tomato::InputChannelComponent>(cam, (uint8_t)0);
    // registry.emplace<tomato::JumpComponent>(cam);

    const auto me = world.CreateEntity();

    registry.emplace<tomato::TransformComponent>(me);

    registry.emplace<tomato::SpeedComponent>(me, (float)2.f);
    registry.emplace<tomato::InputChannelComponent>(me, (uint8_t)0);
    registry.emplace<tomato::JumpComponent>(me);

    registry.emplace<tomato::ColliderComponent>(me, tmt::ColliderType::Cube);

    registry.emplace<tomato::RenderComponent>(me,
                                              tmt::Vector4(1.f, 1.f, 0.f, 1.f),
                                              tmt::GetAssetID(tmt::Mesh::GetName(tmt::Mesh::PrimitiveType::CUBE)),
                                              tmt::GetAssetID(tmt::Shader::PrimitiveName),
                                              //tmt::GetAssetID("assets/WATER_GAME_LOGO.png"));
                                              tmt::GetAssetID(tmt::Texture::PrimitiveName));

    const auto east = world.CreateEntity();

    registry.emplace<tomato::TransformComponent>(east, glm::vec3{5, 0, 0}, glm::vec3{90, 0, 0});

    registry.emplace<tomato::SpeedComponent>(east, (float)1.f);

    registry.emplace<tomato::InputChannelComponent>(east, (uint8_t)1);
    registry.emplace<tomato::JumpComponent>(east);

    registry.emplace<tomato::ColliderComponent>(east, tmt::ColliderType::Cube);

    registry.emplace<tomato::RenderComponent>(east,
                                              tmt::Vector4(0.f, 0.f, 1.f, 1.f),
                                              tmt::GetAssetID(tmt::Mesh::GetName(tmt::Mesh::PrimitiveType::CUBE)),
                                              tmt::GetAssetID(tmt::Shader::PrimitiveName),
                                              tmt::GetAssetID(tmt::Texture::PrimitiveName));

    const auto west = world.CreateEntity();

    registry.emplace<tomato::TransformComponent>(west, glm::vec3{-5, 0, 0}, glm::vec3{-90, 0, 0});

    registry.emplace<tomato::RenderComponent>(west,
        tmt::Vector4(1.f, 1.f, 1.f, 1.f),
        tmt::GetAssetID(tmt::Mesh::GetName(tmt::Mesh::PrimitiveType::SPHERE)),
        tmt::GetAssetID(tmt::Shader::PrimitiveName),
        tmt::GetAssetID(tmt::Texture::PrimitiveName));

    const auto south = world.CreateEntity();

    registry.emplace<tomato::TransformComponent>(south, glm::vec3{0, 0, 5});

    registry.emplace<tomato::RenderComponent>(south,
        tmt::Vector4(1.f, 0.f, 0.f, 1.f),
        tmt::GetAssetID(tmt::Mesh::GetName(tmt::Mesh::PrimitiveType::SPHERE)),
        tmt::GetAssetID(tmt::Shader::PrimitiveName),
        tmt::GetAssetID(tmt::Texture::PrimitiveName));

    const auto north = world.CreateEntity();

    registry.emplace<tomato::TransformComponent>(north, glm::vec3{0, 0, -5});

    registry.emplace<tomato::RenderComponent>(north,
        tmt::Vector4(0.f, 1.f, 0.f, 1.f),
        tmt::GetAssetID(tmt::Mesh::GetName(tmt::Mesh::PrimitiveType::SPHERE)),
        tmt::GetAssetID(tmt::Shader::PrimitiveName),
        tmt::GetAssetID(tmt::Texture::PrimitiveName));

    // const auto floor = world.CreateEntity();
    //
    // registry.emplace<tomato::PositionComponent>(floor, tmt::Vector3{ 0, -0.6, 0 });
    // registry.emplace<tomato::RotationComponent>(floor);
    // registry.emplace<tomato::ScaleComponent>(floor, (glm::vec3{ 30.f, 0.2f, 30.f }));
    // registry.emplace<tomato::WorldMatrixComponent>(floor);
    //
    // registry.emplace<tomato::RenderComponent>(floor,
    //     tmt::Vector4(0.5f, 0.5f, 0.5f, 1.f),
    //     tmt::GetAssetID(tmt::Mesh::GetName(tmt::Mesh::PrimitiveType::CUBE)),
    //     tmt::GetAssetID(tmt::Shader::PrimitiveName),
    //     tmt::GetAssetID(tmt::Texture::PrimitiveName));

    const auto floor = world.CreateEntity();

    registry.emplace<tomato::TransformComponent>(floor, glm::vec3{0, -0.6, 0}, glm::vec3{0.f}, glm::vec3{30.f, 0.2f, 30.f});

    registry.emplace<tomato::RenderComponent>(floor,
        tmt::Vector4(0.5f, 0.5f, 0.5f, 1.f),
        tmt::GetAssetID(tmt::Mesh::GetName(tmt::Mesh::PrimitiveType::CUBE)),
        tmt::GetAssetID(tmt::Shader::PrimitiveName),
        tmt::GetAssetID(tmt::Texture::PrimitiveName));

//    const auto text = world.CreateEntity();
//    registry.emplace<tomato::TextComponent>(text, U"테스트test입니다.123", glm::vec4{ 0.3, 0.7f, 0.9f, 1.0f });
//    registry.emplace<tomato::PositionComponent>(text, tmt::Vector3{ 250.0f, 300.0f, 0.5f });
//    registry.emplace<tomato::ScaleComponent>(text, (glm::vec3{ 0.5f, 0.5f, 1.f }));
}

void TestState::Exit() {}