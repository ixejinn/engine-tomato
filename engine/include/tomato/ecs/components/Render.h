#ifndef TOMATO_SPRITE_H
#define TOMATO_SPRITE_H

#include "tomato/resource/tomato_resource.h"
#include "tomato/tomato_math.h"

namespace tomato
{
	struct RenderComponent
	{
        Vector4 color{1.f, 1.f, 1.f, 1.f};
        AssetId mesh;
        AssetId shader;
		AssetId texture;
	};
}

#endif