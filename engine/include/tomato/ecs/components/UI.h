#ifndef TOMATO_UI_H
#define TOMATO_UI_H

#include "tomato/ecs/tomato_ecs.h"
#include <glm/glm.hpp>

namespace tomato
{
	enum class UISpace
	{
		Screen,
		World
	};

	struct CanvasComponent // root
	{
		UISpace space{ UISpace::Screen };

		glm::vec2 referenceSize{ 1600.f, 900.f };
		glm::vec2 actualSize{ 1600.f, 900.f };
		glm::vec2 scale; // actual / reference

		Entity camera; // optinal
	};

	struct UIComponent
	{
		Entity canvas{ entt::null };
	}; // Tag

	struct RectComponent
	{
		float width;
		float height;

		glm::vec2 pivot{ 0.5f, 0.5f };

		glm::vec2 anchorMin{ 0.5f, 0.5f };
		glm::vec2 anchorMax{ 0.5f, 0.5f };

		glm::vec2 anchoredPosition{ 0.f, 0.f };

		bool dirty{ true };
	};
}

#endif // !TOMATO_UI_H