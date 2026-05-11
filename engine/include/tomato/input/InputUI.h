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
		bool HitTest(const MouseEvent& mouseEvent);
		bool Hover(const MouseMoveEvent& moveEvent);

	private:
		bool PointInRect(glm::vec2 point, glm::vec2 min, glm::vec2 max);
	};
}


#endif // !TOMATO_INPUTUI_H
