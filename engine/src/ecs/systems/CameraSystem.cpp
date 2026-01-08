#include "tomato/ecs/systems/CameraSystem.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/Tags.h"
#include "tomato/Engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <cmath>

tomato::CameraSystem::CameraSystem() : activeCamera_(entt::null) {}

void tomato::CameraSystem::SetActive(World& world, CameraType type)
{
	auto view = world.GetRegistry().view<CameraComponent>();
	for (auto [e, cam] : view.each())
	{
		bool match = false;
		switch (type)
		{
			case CameraType::Main:
				match = world.GetRegistry().any_of<MainCameraTag>(e);
				break;
		}

		cam.active = match;
		if (match)
			activeCamera_ = e;
	}
}

tomato::CameraComponent* tomato::CameraSystem::GetActiveCamera(World& world)
{
	return world.GetRegistry().valid(activeCamera_) ? &world.GetRegistry().get<CameraComponent>(activeCamera_) : nullptr;
}

void tomato::CameraSystem::Update(Engine& engine, const SimContext& ctx)
{
	auto view = engine.GetWorld().GetRegistry().view<CameraComponent, PositionComponent, RotationComponent>();
	for (auto [e, cam, pos, rot] : view.each())
	{
		if (!cam.active) continue;

		//Projection
		//cam.projection = glm::perspective(glm::radians(cam.fov), /*aspect*/(float)(1600.f / 900.f), cam.nearClip, cam.farClip);
		cam.projection = glm::perspective(glm::radians(45.f), /*aspect*/(float)(1600.f / 900.f), 0.1f, 100.f);

		//View
		glm::vec3 front(0.f, 0.f, -1.f);
		front.x = cos(glm::radians(rot.rotation.y)) * cos(glm::radians(rot.rotation.x));
		front.y = sin(glm::radians(rot.rotation.x));
		front.z = sin(glm::radians(rot.rotation.y)) * cos(glm::radians(rot.rotation.x));
		front = glm::normalize(front);

		//glm::vec3 cameraTarget = pos.position + glm::normalize(front);
		glm::vec3 cameraTarget(0.f, 0.f, 0.f);
		glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
		glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
		//glm::vec3 up = glm::normalize(glm::cross(right, front));

		cam.view = glm::lookAt(pos.position, cameraTarget, worldUp);
	}
}