#ifndef TOMATO_RENDER_H
#define TOMATO_RENDER_H

#include "System.h"
#include "tomato/resource/tomato_resource.h"
#include "tomato/resource/TextRenderer.h"

namespace tomato
{
	class RenderSystem : public System
	{
	public:
        RenderSystem();

		void Update(Engine& engine, const SimContext& ctx) override;

	private:
        AssetId curMesh_;
        AssetId curShader_;
        AssetId curTexture_;
		AssetId curFont_;

		TextRenderer textRenderer_;
	};
}

#endif