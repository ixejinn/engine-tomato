#ifndef TOMATO_UISYSTEM_H
#define TOMATO_UISYSTEM_H

#include "System.h"

namespace tomato
{
	class UISystem : public System
	{
	public:
		UISystem();
		void Update(Engine& engine, const SimContext& ctx) override;

	private:
	};

}

#endif // !TOMATO_UISYSTEM_H
