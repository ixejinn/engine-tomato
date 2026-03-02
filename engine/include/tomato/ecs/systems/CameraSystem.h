#ifndef TOMATO_CAMERASYS_H
#define TOMATO_CAMERASYS_H

#include "System.h"

namespace tomato
{
	class CameraSystem : public System
	{
	public:
		void Update(Engine& engine, const SimContext& ctx) override;
	};
}

#endif