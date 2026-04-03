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

			glm::vec2 scaleFactor = canvas.actualSize / canvas.referenceSize;

			glm::vec2 anchorCenter = (rect.anchorMin + rect.anchorMax) * 0.5f;
			glm::vec2 anchorPos = canvas.referenceSize * anchorCenter;

			glm::vec2 pos = anchorPos + rect.anchoredPosition;
			pos *= scaleFactor;

			glm::vec2 size = glm::vec2(rect.width, rect.height) * scaleFactor;
			pos -= size * rect.pivot;

			rect.position = glm::vec3(pos, 0.f);
			rect.scale = glm::vec3(size, 1.f);
		}
	}

}
