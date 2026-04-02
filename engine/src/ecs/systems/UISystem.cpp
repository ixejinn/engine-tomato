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
		auto view = engine.GetWorld().GetRegistry().view<TransformComponent, RectComponent, UIComponent>();
		
		for (auto [e, transform, rect, ui] : view.each())
		{
			if (ui.canvas == entt::null)
				continue;

			auto& canvas = engine.GetWorld().GetRegistry().get<CanvasComponent>(ui.canvas);

			glm::vec2 anchorPos = canvas.scale * rect.anchorMin;
			glm::vec2 pos = anchorPos + rect.anchoredPosition;
			pos -= glm::vec2(rect.width, rect.height) * rect.pivot;

			transform.position = glm::vec3(pos, 0.f);
			transform.scale = glm::vec3(rect.width, rect.height, 1.f);
		}
	}

}
