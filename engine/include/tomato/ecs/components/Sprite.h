#ifndef TOMATO_SPRITE_H
#define TOMATO_SPRITE_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace tomato
{
	struct SpriteComponent
	{
		uint32_t shader_id{ 0 };
		uint32_t texture_id{ 0 };
	};
}

#endif