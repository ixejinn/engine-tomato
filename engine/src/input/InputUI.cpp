#include "tomato/input/InputUI.h"
#include "tomato/Engine.h"
#include "tomato/tomato_sim.h"
#include "tomato/services/WindowService.h"
#include "tomato/services/InputService.h"
#include "tomato/ecs/components/UI.h"
#include "tomato/ecs/components/Render.h"
#include <GLFW/glfw3.h>

namespace tomato
{
	bool InputUI::HitTest(const MouseEvent& mouseEvent)
	{
		GLFWwindow* w = glfwGetCurrentContext();
		auto* engine = static_cast<WindowData*>(glfwGetWindowUserPointer(w))->engine;
		auto& r = engine->GetWorld().GetRegistry();
		float windowHeight = (float)engine->GetWindowService().GetHeight();

		double x, y;
		InputService::GetMouseCursorPos(w, &x, &y);
		auto* uiCtx = r.ctx().find<UIContext>();
		if (uiCtx == nullptr)
			return true;
		
		for (auto it = uiCtx->drawList.rbegin(); it != uiCtx->drawList.rend(); ++it)
		{
			if (!r.all_of<SelectableComponent>(*it)) continue;

			auto& rect = r.get<RectTransformComponent>(*it);
			auto& selectable = r.get<SelectableComponent>(*it);
			auto& render = r.get<RenderComponent>(*it);

			if (selectable.interactable)
			{
				if (PointInRect(glm::vec2(x, windowHeight - y), rect.screenRect.min, rect.screenRect.max))
				{
					if (mouseEvent.action == KeyAction::PRESS)
						render.color = selectable.pressedColor;

					if (mouseEvent.action == KeyAction::RELEASE)
					{
						render.color = selectable.normalColor;

						if(selectable.enter)
							selectable.enter(MouseEnterEvent{ *it, &r });
					}

					break;
				}
				else
				{
					render.color = selectable.normalColor;
					break;
				}
			}
		}

		return false;
	}

	bool InputUI::Hover(const MouseMoveEvent& moveEvent)
	{
		GLFWwindow* w = glfwGetCurrentContext();
		auto* engine = static_cast<WindowData*>(glfwGetWindowUserPointer(w))->engine;
		auto& r = engine->GetWorld().GetRegistry();
		float windowHeight = (float)engine->GetWindowService().GetHeight();

		double x{ moveEvent.xPos }, y{ moveEvent.yPos };

		auto* uiCtx = r.ctx().find<UIContext>();
		if (uiCtx == nullptr)
			return true;

		for (auto it = uiCtx->drawList.rbegin(); it != uiCtx->drawList.rend(); ++it)
		{
			if (!r.all_of<SelectableComponent>(*it)) continue;

			auto& rect = r.get<RectTransformComponent>(*it);
			auto& button = r.get<SelectableComponent>(*it);
			auto& render = r.get<RenderComponent>(*it);

			if (button.interactable)
			{
				if (PointInRect(glm::vec2(x, windowHeight - y), rect.screenRect.min, rect.screenRect.max))
				{
					render.color = button.highlightedColor;
					break;
				}
				else
				{
					render.color = button.normalColor;
					break;
				}
			}
		}
		return false;
	}

	bool InputUI::PointInRect(glm::vec2 point, glm::vec2 min, glm::vec2 max)
	{
		return
			point.x >= min.x &&
			point.x <= max.x &&
			point.y >= min.y &&
			point.y <= max.y;
	}
}
