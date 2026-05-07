#include "tomato/ecs/systems/UISystem.h"
#include "tomato/Engine.h"
#include "tomato/services/WindowService.h"
#include "tomato/services/InputService.h"
#include "tomato/tomato_sim.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/UI.h"
#include "tomato/ecs/components/Text.h"
#include "tomato/ecs/components/Camera.h"
#include "tomato/ecs/components/Tags.h"
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

		UpdateRectTransform(engine);

		
		HitTest(engine);
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
					auto& text = r.get<TextComponent>(entity);
					if (text.dirty)
					{
						text.codepoints = UTF8ToUTF32(text.text);
						Font* font = AssetRegistry<Font>::GetInstance().Get(text.font);

						rect.sizeDelta = font->MeasureText(text.codepoints, text.fontSize / 64.f);
						text.dirty = false;
					}
				}
				
				// World Name Label
				if (r.all_of<TargetComponent>(entity))
				{
					auto& target = r.get<TargetComponent>(entity);
					auto& targetTransform = r.get<PositionComponent>(target.target);

					auto viewProjection = glm::mat4(1.f);
					CameraComponent* cam{ nullptr };

					auto curCam = engine.GetCurrentCamera();
					if (curCam == entt::null)
					{
						curCam = engine.GetWorld().GetRegistry().view<MainCameraTag>().front();

						if (curCam == entt::null)
						{
							TMT_WARN << "Main camera not present";
							continue;
						}
						else
							engine.SetCurrentCamera(curCam);
					}

					if ((cam = engine.GetWorld().GetRegistry().try_get<CameraComponent>(curCam)))
						viewProjection = cam->viewProjection;
					
					glm::vec3 screenPos = WorldToScreen(targetTransform.position, viewProjection, 1600.f, 900.f);
					rect.position = screenPos + target.headOffset;

					rect.computedSize = rect.sizeDelta;
					continue;
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
	void UISystem::HitTest(Engine& engine)
	{
		auto& r = engine.GetWorld().GetRegistry();
		double x, y;
		InputService::GetMouseCursorPos(engine.GetWindowService().GetHandle(), &x, &y);
		//std::cout << x << ", " << y << '\n';

		auto* uiCtx = r.ctx().find<UIContext>();
		if (uiCtx == nullptr)
		{
			std::cout << "NULL DRAWLIST\n";
			return;
		}
		
		//for (auto it = uiCtx->drawList.end(); it != uiCtx->drawList.begin(); it--)
		//{
		//	auto& rect = r.get<RectTransformComponent>(*it);

		//	glm::vec2 rectMin, rectMax;

		//}
	}

	glm::vec3 UISystem::WorldToScreen(const glm::vec3& worldPos, const glm::mat4& viewProjection, float screenWidth, float screenHeight)
	{
		//World -> Clip
		glm::vec4 clipPos = viewProjection * glm::vec4(worldPos, 1.0f);

		//Perspective divide (Clip -> NDC)
		glm::vec3 ndc = glm::vec3(clipPos) / clipPos.w;

		//NDC -> Screen
		glm::vec2 screenPos{};
		screenPos.x = (ndc.x * 0.5f + 0.5f) * screenWidth;
		screenPos.x -= screenWidth * 0.5f;

		screenPos.y = (ndc.y * 0.5f + 0.5f) * screenHeight;
		screenPos.y -= screenHeight * 0.5f;

		return glm::vec3(screenPos, 0.f);
	}
}
