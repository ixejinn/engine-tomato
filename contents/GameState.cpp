#include "GameState.h"
#include "tomato/tomato.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/Rigidbody.h"
#include "tomato/ecs/components/Movement.h"
#include "tomato/ecs/components/Render.h"
#include "tomato/ecs/components/Camera.h"
#include "tomato/ecs/components/Tags.h"
#include "tomato/resource/AssetRegistry.h"
#include "tomato/resource/render/Mesh.h"
#include "tomato/resource/render/Texture.h"
#include "tomato/resource/render/Shader.h"
#include <iostream>

void GameState::Init(tomato::World& world)
{
//	std::cout << "GAME STATE\n";
//
//    tmt::Registry& registry = world.GetRegistry();
//
//    const auto cam = world.CreateEntity();
//    registry.emplace<tomato::PositionComponent>(cam, glm::vec3(0.f, 15.f, 30.f));
//    registry.emplace<tomato::RotationComponent>(cam, glm::vec3(-30.f, 0.f, 0.f));
//    registry.emplace<tomato::CameraComponent>(cam);
//    registry.emplace<tomato::MainCameraTag>(cam);
}

void GameState::Exit() {}