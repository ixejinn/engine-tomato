#ifndef TOMATO_SPRITE_H
#define TOMATO_SPRITE_H

#include "tomato/resource/tomato_resource.h"
#include "tomato/tomato_math.h"
#include "tomato/resource/AssetRegistry.h"
#include "tomato/resource/render/Mesh.h"
#include "tomato/resource/render/Texture.h"
#include "tomato/resource/render/Shader.h"

namespace tomato
{
	struct RenderComponent
	{
        Vector4 color{1.f, 1.f, 1.f, 1.f};
		AssetId mesh{ GetAssetID(Mesh::GetName(Mesh::PrimitiveType::CUBE)) };
		AssetId shader{ GetAssetID(Shader::PrimitiveName) };
		AssetId texture{ GetAssetID(Texture::PrimitiveName) };
	};
}

#endif