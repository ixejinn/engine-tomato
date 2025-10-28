#ifndef TOMATO_RENDER_H
#define TOMATO_RENDER_H

#include <unordered_map>
#include <atomic>
#include <memory>

#include "System.h"
#include "tomato/Logger.h"
#include "tomato/render/Mesh.h"
#include "tomato/render/Shader.h"
#include "tomato/render/Texture.h"

using ResourceID = uint32_t;

struct ShaderInfo
{
	std::string tag;
	std::string vertexPath;
	std::string fragmentPath;
};

struct TextureInfo
{
	std::string tag;
	std::string path;
};

namespace tomato
{
	class RenderSystem : public System
	{
	public:
		RenderSystem();

		void RegisterShader(const std::string& tag, const char* vertexPath, const char* fragmentPath);
		Shader* GetShader(ResourceID id);

		void RegisterTexture(const std::string& tag, const std::string& path, ImageFormat format = ImageFormat::RGBA8);
		Texture* GetTexture(ResourceID id);

		void Init();

		void Update(Engine& engine, const SimContext& ctx) override;
		void Update(const Engine& engine, const SimContext& ctx) override;

	private:
		Mesh mesh;

		std::atomic<uint32_t> shaderCounter_{ 0 };
		std::unordered_map<ResourceID, std::unique_ptr<Shader>> shaders_;
		std::unordered_map<std::string, ResourceID> tagToShaderID_;

		std::atomic<uint32_t> textureCounter_{ 0 };
		std::unordered_map<ResourceID, std::unique_ptr<Texture>> textures_;
		std::unordered_map<std::string, ResourceID> tagToTextureID_;

	};
}

#endif