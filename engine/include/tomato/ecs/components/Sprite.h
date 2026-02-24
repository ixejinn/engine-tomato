#ifndef TOMATO_SPRITE_H
#define TOMATO_SPRITE_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "tomato/resource/tomato_resource.h"

namespace tomato
{
	struct SpriteComponent
	{
        AssetId shader_id{ 0 };
		AssetId texture_id{ 0 };
	};
}

#endif