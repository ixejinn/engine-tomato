#ifndef TOMATO_UI_H
#define TOMATO_UI_H

#include "tomato/ecs/tomato_ecs.h"
#include <glm/glm.hpp>

namespace tomato
{
	enum class RenderMode
	{
		ScreenOverlay,
		ScreenCamera,
		World
	};

	struct CanvasComponent // root
	{
		RenderMode mode{ RenderMode::ScreenOverlay };

		glm::vec2 referenceSize{ 1600.f, 900.f };
		glm::vec2 actualSize{ 1600.f, 900.f };

		Entity camera; // optinal
	};

	struct UIComponent
	{
		Entity canvas{ entt::null };
	}; // Tag

	struct RectTransformComponent
	{
		glm::vec2 anchoredPosition{ 0.f, 0.f }; // anchorMin == anchorMax, posX, posY

		glm::vec2 offsetMin{ 0.f, 0.f }; // stretch, left, bottom
		glm::vec2 offsetMax{ 0.f, 0.f }; //			 right, top

		glm::vec2 sizeDelta{ 1600.f, 900.f };

		glm::vec2 anchorMin{ 0.0f, 0.0f };
		glm::vec2 anchorMax{ 0.0f, 0.0f };

		glm::vec2 pivot{ 0.5f, 0.5f };

		// for calculation
		glm::vec2 computedSize{ 0.f, 0.f };
		Vector3 position{ 0.f, 0.f, 0.f };
		Vector3 scale{ 1.f, 1.f, 1.f };
		Vector3 rotation{ 0.f, 0.f, 0.f };

		glm::mat4 local_matrix{ 1.f };
		glm::mat4 world_matrix{ 1.f };
		glm::mat4 model_matrix{ 1.f };

		Entity parent{ entt::null };
		std::vector<Entity> children;

		bool dirty{ true };

		void SetParent(Entity e) { parent = e; }
	};
}

#endif // !TOMATO_UI_H