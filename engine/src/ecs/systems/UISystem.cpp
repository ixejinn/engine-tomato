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
		BuildDrawList(engine);
#if 1
		auto view = engine.GetWorld().GetRegistry().view<HierarchyComponent, RectTransformComponent, UIComponent>();
		
		for (auto [e, hierarchy, rect, ui] : view.each())
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

			auto& parentRect = engine.GetWorld().GetRegistry().get<RectTransformComponent>(hierarchy.parent);

			glm::vec2 scaleFactor = canvas.actualSize / canvas.referenceSize;
			glm::vec2 parentSize = (hierarchy.parent == entt::null) ? canvas.referenceSize : parentRect.computedSize;
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
				glm::vec2 anchorPosMin = parentSize * rect.anchorMin;
				glm::vec2 anchorPosMax = parentSize * rect.anchorMax;

				glm::vec2 finalLocalMin = (anchorPosMin - parentPivotPos) + rect.offsetMin;
				glm::vec2 finalLocalMax = (anchorPosMax - parentPivotPos) + rect.offsetMax;

				rect.computedSize = finalLocalMax - finalLocalMin;

				glm::vec2 localPos = finalLocalMin + (rect.computedSize * rect.pivot);
				rect.position = glm::vec3(localPos * scaleFactor, 0.f);
			}
		}
#else 0
		UpdateRectTransform(engine);
#endif
	}

	void UISystem::Traverse(Engine& engine, Entity e)
	{
		drawList.push_back(e);
		
		auto& hierarchy = engine.GetWorld().GetRegistry().get<HierarchyComponent>(e);
		for (auto child : hierarchy.children)
			Traverse(engine, child);
	}

	void UISystem::BuildDrawList(Engine& engine)
	{
		drawList.clear();

		std::vector<Entity> canvases;

		auto canvasView = engine.GetWorld().GetRegistry().view<CanvasComponent>();
		for (auto canvas : canvasView)
			canvases.push_back(canvas);
		
		std::sort(canvases.begin(), canvases.end(),
			[&](Entity a, Entity b)
			{
				return engine.GetWorld().GetRegistry().get<CanvasComponent>(a).sortOrder <
					engine.GetWorld().GetRegistry().get<CanvasComponent>(b).sortOrder;
			});

		for (auto canvas : canvases)
			Traverse(engine, canvas);
	}

	void UISystem::UpdateRectTransform(Engine& engine)
	{
		if (drawList.empty())
			return;

		auto& r = engine.GetWorld().GetRegistry();
		CanvasComponent* currentCanvas = nullptr;
		for (auto entity : drawList)
		{
			auto& hierarchy = r.get<HierarchyComponent>(entity);
			auto& rect = r.get<RectTransformComponent>(entity);

			// entity is canvas(root).
			if (hierarchy.parent == entt::null)
			{
				auto& canvas = r.get<CanvasComponent>(entity);
				currentCanvas = &canvas;

				rect.computedSize = canvas.actualSize;
				rect.position = glm::vec3(rect.computedSize * rect.pivot, 0.f);
				rect.scale = glm::vec3(1.f, 1.f, 1.f);

				continue;
			}

			// children
			auto& parentRect = r.get<RectTransformComponent>(hierarchy.parent);

			glm::vec2 scaleFactor = currentCanvas->actualSize / currentCanvas->referenceSize;
			glm::vec2 parentSize = parentRect.computedSize;
			glm::vec2 parentPivotPos = parentSize * parentRect.pivot;

			if (rect.anchorMin == rect.anchorMax) // anchor point
			{
				glm::vec2 anchorPos = parentSize * rect.anchorMin;
				glm::vec2 localPos = (anchorPos - parentPivotPos) + rect.anchoredPosition;

				rect.computedSize = rect.sizeDelta;
				rect.position = glm::vec3(localPos * scaleFactor, 0.f);
			}
			else // anchor stretch
			{
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
