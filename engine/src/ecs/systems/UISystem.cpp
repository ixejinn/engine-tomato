#include "tomato/ecs/systems/UISystem.h"
#include "tomato/Engine.h"
#include "tomato/tomato_sim.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/UI.h"
#include "tomato/Logger.h"

#include "tomato/RegistryEntry.h"
REGISTER_SYSTEM(tomato::SystemPhase::UI, UISystem)

namespace tomato
{
	UISystem::UISystem()
	{
	}
	void UISystem::Update(Engine& engine, const SimContext& ctx)
	{
		auto view = engine.GetWorld().GetRegistry().view<RectTransformComponent, UIComponent>();
		
		for (auto [e, rect, ui] : view.each())
		{
			if (ui.canvas == entt::null)
			{
				std::cout << "[UISystem] Can not found canvas.\n";
				continue;
			}

			auto& canvas = engine.GetWorld().GetRegistry().get<CanvasComponent>(ui.canvas);
			if (ui.canvas == e)
			{
				rect.computedSize = canvas.actualSize;
				rect.position = glm::vec3(rect.computedSize * rect.pivot, 0.f);
				rect.scale = glm::vec3(1.f, 1.f, 1.f);
				
				continue;
			}

			auto& parentRect = engine.GetWorld().GetRegistry().get<RectTransformComponent>(rect.parent);

			glm::vec2 scaleFactor = canvas.actualSize / canvas.referenceSize;
			glm::vec2 parentSize = (rect.parent == entt::null) ? canvas.referenceSize : parentRect.computedSize;
			glm::vec2 parentPivotPos = parentSize * parentRect.pivot;
			
			if (rect.anchorMin == rect.anchorMax) // anchor point
			{
				//std::cout << "point : min(" << rect.anchorMin.x << ", " << rect.anchorMin.y <<") max(" << rect.anchorMax.x << ", " << rect.anchorMax.y << ")\n";
				//std::cout << "pivot : (" << rect.pivot.x << ", " << rect.pivot.y << ") sizeDelta : (" << rect.sizeDelta.x << ", " << rect.sizeDelta.y << ")\n";
				glm::vec2 anchorPos = parentSize * rect.anchorMin;
				glm::vec2 localPos = (anchorPos - parentPivotPos) + rect.anchoredPosition;

				rect.computedSize = rect.sizeDelta;
				rect.position = glm::vec3(localPos * scaleFactor, 0.f);

				//std::cout << "position(" << localPos.x << ", " << localPos.y << ") size(" << rect.sizeDelta.x << ", " << rect.sizeDelta.y << ")\n";
			}
			else // anchor stretch
			{
				//std::cout << "stretch\n";
				glm::vec2 anchorPosMin = parentSize * rect.anchorMin;
				glm::vec2 anchorPosMax = parentSize * rect.anchorMax;

				glm::vec2 finalLocalMin = (anchorPosMin - parentPivotPos) + rect.offsetMin;
				glm::vec2 finalLocalMax = (anchorPosMax - parentPivotPos) + rect.offsetMax;

				rect.computedSize = finalLocalMax - finalLocalMin;

				glm::vec2 localPos = finalLocalMin + (rect.computedSize * rect.pivot);
				rect.position = glm::vec3(localPos * scaleFactor, 0.f);
			}
		}
	}

}
