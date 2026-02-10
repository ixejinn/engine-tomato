#ifndef TOMATO_RENDER_H
#define TOMATO_RENDER_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace tomato
{
    // Stores handle of ResourceRegistry
    struct RenderComponent
    {
        uint32_t mesh{0};
        uint32_t shader{0};
    };

    struct TextureComponent { uint32_t texture{0}; };

	struct SpriteComponent
	{
		uint32_t shader_id{ 0 };
		uint32_t texture_id{ 0 };
	};
}

#endif