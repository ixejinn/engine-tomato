#ifndef TOMATO_RENDER_H
#define TOMATO_RENDER_H

#include "tomato/resource/tomato_resource.h"
#include "tomato/tomato_math.h"

namespace tomato
{
    // Stores asset ID
    struct RenderComponent
    {
        Vector4 color{1.f, 1.f, 1.f, 1.f};
        AssetId mesh{0};
        AssetId shader{0};
        AssetId texture{0};
    };

    struct TextureComponent { AssetId texture{0}; };

	struct SpriteComponent
	{
		uint32_t shader_id{ 0 };
		uint32_t texture_id{ 0 };
	};
}

#endif