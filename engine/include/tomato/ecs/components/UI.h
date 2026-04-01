#ifndef TOMATO_UI_H
#define TOMATO_UI_H

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
		UISpace space;
	};

	struct UIComponent{}; // Tag

	struct RectComponent
	{
		float width;
		float height;

		glm::vec2 pivot;
		glm::vec2 anchorMin;
		glm::vec2 anchorMax;
	};
}

#endif // !TOMATO_UI_H