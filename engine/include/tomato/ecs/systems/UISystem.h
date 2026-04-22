#ifndef TOMATO_UISYSTEM_H
#define TOMATO_UISYSTEM_H

#include "System.h"
#include "tomato/ecs/tomato_ecs.h"

namespace tomato
{
	class UISystem : public System
	{
	public:
		UISystem();
		void Update(Engine& engine, const SimContext& ctx) override;

	private:
		void Traverse(Engine& engine, Entity e);
		void BuildDrawList(Engine& engine);
		void UpdateRectTransform(Engine& engine);

		std::vector<Entity> drawList;
	};

}

#endif // !TOMATO_UISYSTEM_H
