#ifndef TOMATO_INPUTUI_H
#define TOMATO_INPUTUI_H

#include "tomato/input/InputTypes.h"
#include "tomato/event/EventSignal.h"
#include "tomato/tomato_math.h"

namespace tomato
{
	struct ButtonHoverEvent
	{
		entt::entity e;
	};

	class InputUI
	{
	public:
		bool OnClick(const MouseEvent& mouseEvent);
		bool OnHover(const MouseMoveEvent& moveEvent);

	private:
		bool PointInRect(glm::vec2 point, glm::vec2 min, glm::vec2 max);
		entt::entity PickSelectable(glm::vec2 point);
	};
}


#endif // !TOMATO_INPUTUI_H
