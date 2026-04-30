#include "tomato/ecs/systems/UISystem.h"
#include "tomato/Engine.h"
#include "tomato/tomato_sim.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/UI.h"
#include "tomato/ecs/components/Text.h"
#include "tomato/resource/AssetRegistry.h"
#include "tomato/resource/render/Font.h"
#include "tomato/utils/Utf.h"
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
#if 0
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
#else 1
		UpdateRectTransform(engine);
#endif
	}

	void UISystem::Traverse(Engine& engine, Entity e, std::vector<Entity>& drawList)
	{

		auto& ui = engine.GetWorld().GetRegistry().get<UIComponent>(e);
		//std::cout << ui.type << " ";
		drawList.push_back(e);
		
		auto& hierarchy = engine.GetWorld().GetRegistry().get<HierarchyComponent>(e);
		for (auto child : hierarchy.children)
			Traverse(engine, child, drawList);
	}

	void UISystem::BuildDrawList(Engine& engine)
	{
		auto* uiCtx = engine.GetWorld().GetRegistry().ctx().find<UIContext>();
		if (uiCtx == nullptr)
		{
			std::cout << "NULL DRAWLIST\n";
			engine.GetWorld().GetRegistry().ctx().emplace<UIContext>();
			uiCtx = engine.GetWorld().GetRegistry().ctx().find<UIContext>();
		}

		std::vector<Entity> canvases, drawList;

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
			Traverse(engine, canvas, drawList);
		//std::cout << '\n';

		uiCtx->drawList.clear();
		uiCtx->drawList = std::move(drawList);
	}

	void UISystem::UpdateRectTransform(Engine& engine)
	{
		auto& r = engine.GetWorld().GetRegistry();
		auto& uiCtx = r.ctx().get<UIContext>();

		if (uiCtx.drawList.empty())
			return;

		CanvasComponent* currentCanvas = nullptr;
		for (auto entity : uiCtx.drawList)
		{
			auto& hierarchy = r.get<HierarchyComponent>(entity);

			// entity is canvas(root).
			if (hierarchy.parent == entt::null)
			{
				currentCanvas = &r.get<CanvasComponent>(entity);

				auto& rect = r.get<RectTransformComponent>(entity);
				rect.computedSize = currentCanvas->actualSize;
				rect.position = glm::vec3(rect.computedSize * rect.pivot, 0.f);
				rect.scale = glm::vec3(1.f);

				break;
			}
		}

		if (!currentCanvas) return;
		for (auto entity : uiCtx.drawList)
		{
			auto& hierarchy = r.get<HierarchyComponent>(entity);
			if (hierarchy.parent == entt::null)
				continue;

			// children
			auto& rect = r.get<RectTransformComponent>(entity);
			auto& parentRect = r.get<RectTransformComponent>(hierarchy.parent);
			auto& ui = r.get<UIComponent>(entity);


			glm::vec2 scaleFactor = currentCanvas->actualSize / currentCanvas->referenceSize;
			glm::vec2 parentSize = parentRect.computedSize;
			glm::vec2 parentPivotPos = parentSize * parentRect.pivot;

			if (rect.anchorMin == rect.anchorMax) // anchor point
			{
				glm::vec2 anchorPos = parentSize * rect.anchorMin;
				glm::vec2 localPos = (anchorPos - parentPivotPos) + rect.anchoredPosition;

				if (ui.type == 2)
				{
					auto& text = engine.GetWorld().GetRegistry().get<TextComponent>(entity);
					if (text.dirty)
					{
						text.codepoints = UTF8ToUTF32(text.text);
						Font* font = AssetRegistry<Font>::GetInstance().Get(text.font);

						rect.sizeDelta = font->MeasureText(text.codepoints, text.fontSize / 64.f);
						text.dirty = false;
					}
				}
				
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
