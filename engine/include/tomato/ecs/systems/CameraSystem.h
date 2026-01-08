#ifndef TOMATO_CAMERASYS_H
#define TOMATO_CAMERASYS_H

#include "System.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/components/Camera.h"

namespace tomato
{
	enum class CameraType
	{
		Main,
	};

	class CameraSystem : public System
	{
	public:
		CameraSystem();

		template<typename TTag>
		static void SetActive(World& world)
		{
			auto view = world.GetRegistry().view<CameraComponent>();
			for (auto [e, cam] : view.each())
			{
				if (world.GetRegistry().any_of<TTag>(e))
					cam.active = true;
				else
					cam.active = false;
			}
		}

		void SetActive(World& world, CameraType type);
		CameraComponent* GetActiveCamera(World& world);
		Entity GetActiveCameraEntity() const { return activeCamera_; }

		void Update(Engine& engine, const SimContext& ctx) override;
	
	private:
        Entity activeCamera_;
		//zoom
	};
}

#endif