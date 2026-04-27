#ifndef TOMATO_UI_H
#define TOMATO_UI_H

#include "tomato/ecs/tomato_ecs.h"
#include <glm/glm.hpp>

namespace tomato
{
	enum class RenderMode
	{
		ScreenOverlay,
		ScreenCamera,
		World
	};

	struct CanvasComponent // root
	{
		RenderMode mode{ RenderMode::ScreenOverlay };

		glm::vec2 referenceSize{ 1600.f, 900.f };
		glm::vec2 actualSize{ 1600.f, 900.f };

		Entity camera; // optinal
		int sortOrder{ 0 };
	};

	struct UIComponent
	{
		Entity canvas{ entt::null };
		int type{ 0 };
	}; // Tag

	struct RectTransformComponent
	{
		glm::vec2 anchoredPosition{ 0.f, 0.f }; // anchorMin == anchorMax, posX, posY

		glm::vec2 offsetMin{ 0.f, 0.f }; // stretch, left, bottom
		glm::vec2 offsetMax{ 0.f, 0.f }; //			 right, top

		glm::vec2 sizeDelta{ 1600.f, 900.f };

		glm::vec2 anchorMin{ 0.0f, 0.0f };
		glm::vec2 anchorMax{ 0.0f, 0.0f };

		glm::vec2 pivot{ 0.5f, 0.5f };

		// for calculation
		glm::vec2 computedSize{ 0.f, 0.f };
		Vector3 position{ 0.f, 0.f, 0.f };
		Vector3 scale{ 1.f, 1.f, 1.f };
		Vector3 rotation{ 0.f, 0.f, 0.f };

		glm::mat4 local_matrix{ 1.f };
		glm::mat4 world_matrix{ 1.f };
		glm::mat4 model_matrix{ 1.f };

		//bool dirty{ true };
	};

	struct HierarchyComponent
	{
		Entity parent{ entt::null };
		std::vector<Entity> children;
	};

	inline void SetParent(World& world, Entity child, Entity parent)
	{
		entt::registry& r = world.GetRegistry();

		auto& childH = r.get<HierarchyComponent>(child);
		if (childH.parent != entt::null)
		{
			auto& oldParentH = r.get<HierarchyComponent>(childH.parent);
			auto& siblings = oldParentH.children;

			siblings.erase(std::remove(siblings.begin(), siblings.end(), child), siblings.end());
		}

		childH.parent = parent;
		if (parent != entt::null)
		{
			auto& newParentH = r.get<HierarchyComponent>(parent);
			newParentH.children.push_back(child);
		}
	}

}

#endif // !TOMATO_UI_H