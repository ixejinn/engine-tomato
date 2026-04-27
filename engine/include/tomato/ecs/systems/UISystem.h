#ifndef TOMATO_UISYSTEM_H
#define TOMATO_UISYSTEM_H

#include "System.h"
#include "tomato/ecs/tomato_ecs.h"
//#include "tomato/tomato_sim.h"

namespace tomato
{
	class UISystem : public System
	{
	public:
		UISystem();
		void Update(Engine& engine, const SimContext& ctx) override;

	private:
		void Traverse(Engine& engine, Entity e, std::vector<Entity>& drawList);
		void BuildDrawList(Engine& engine);
		void UpdateRectTransform(Engine& engine);

		//std::vector<Entity> drawList;
	};

}

#endif // !TOMATO_UISYSTEM_H
