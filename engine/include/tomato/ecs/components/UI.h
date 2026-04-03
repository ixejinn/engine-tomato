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
		Vector3 position{ 0.f, 0.f, 0.f };
		Vector3 scale{ 1.f, 1.f, 1.f };
		Vector3 rotation{ 0.f, 0.f, 0.f };

		float width{ 800.f };
		float height{ 450.f };

		glm::vec2 pivot{ 0.5f, 0.5f };

		glm::vec2 anchorMin{ 0.5f, 0.5f };
		glm::vec2 anchorMax{ 0.5f, 0.5f };

		glm::vec2 anchoredPosition{ 0.f, 0.f };

		glm::mat4 local_matrix;
		glm::mat4 world_matrix;

		Entity parent{ entt::null };
		std::vector<Entity> children;

		bool dirty{ true };
	};
}

#endif // !TOMATO_UI_H