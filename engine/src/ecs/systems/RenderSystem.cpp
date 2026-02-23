#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tomato/ecs/systems/RenderSystem.h"
#include "tomato/Engine.h"
#include "tomato/tomato_sim.h"
#include "tomato/ecs/components/Sprite.h"
#include "tomato/ecs/components/Transform.h"
//#include "tomato/ecs/components/Camera.h"

#include "tomato/RegistryEntry.h"
REGISTER_SYSTEM(tomato::SystemPhase::RENDER, RenderSystem)

namespace tomato
{
	RenderSystem::RenderSystem()
	{
		Init();

        mesh = new Mesh(Mesh::PrimitiveType::PLAIN);
	}

	void RenderSystem::RegisterShader(const std::string& tag, const char* vertexPath, const char* fragmentPath)
	{
		auto itTag = tagToShaderID_.find(tag);
		if (itTag != tagToShaderID_.end())
		{
			ResourceID id = itTag->second;
			if (shaders_.find(id) != shaders_.end())
			{
				TMT_ERR << "[shader] " << tag << " is already registered.";
				return;
			}
		}

		ResourceID id = shaderCounter_.fetch_add(1);
		shaders_[id] = std::make_unique<Shader>(vertexPath, fragmentPath);
		tagToShaderID_[tag] = id;

		TMT_INFO << "[shader] " << tag << " is successfully registered. id = " << id;
	}

	Shader* RenderSystem::GetShader(ResourceID id)
	{
		auto shader = shaders_.find(id);
		if (shader != shaders_.end())
			return shader->second.get();

		return nullptr;
	}

	void RenderSystem::RegisterTexture(const std::string& tag, const std::string& path, ImageFormat format)
	{
		auto itTag = tagToTextureID_.find(tag);
		if (itTag != tagToTextureID_.end())
		{
			ResourceID id = itTag->second;
			if (textures_.find(id) != textures_.end())
			{
				TMT_ERR << "[texture] " << tag << " is already registered.";
				return;
			}
		}

		ResourceID id = textureCounter_.fetch_add(1);
		textures_[id] = std::make_unique<Texture>(path, format);
		tagToTextureID_[tag] = id;

		TMT_INFO << "[texture] " << tag << " is successfully registered. id = " << id;
	}

	Texture* RenderSystem::GetTexture(ResourceID id)
	{
		auto tex = textures_.find(id);
		if (tex != textures_.end())
			return tex->second.get();

		return nullptr;
	}

	void RenderSystem::Init()
	{
		//Shaders
		RegisterShader("default", "assets/shader.vs", "assets/shader.fs");

		//Textures
		RegisterTexture("PWLogo", "assets/WATER_GAME_LOGO.png");
	}

	void RenderSystem::Update(Engine& engine, const SimContext& ctx) { TMT_INFO << "Render Update";  }
	void RenderSystem::Update(const Engine& engine, const SimContext& ctx)
	{
		//TMT_INFO << "const Render Update";

		auto view = engine.GetWorld().GetRegistry().view<SpriteComponent, WorldMatrixComponent>();
		for (auto [e, sprite, mtx] : view.each())
		{
			//TMT_INFO << sprite.shader_id << "," << sprite.texture_id;

			Shader* shader = GetShader(sprite.shader_id);
			Texture* texture = GetTexture(sprite.texture_id);
			
			if (texture && shader)
			{
				shader->Use();
				glDisable(GL_DEPTH_TEST);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture->GetID());

				GLint texLoc = glGetUniformLocation(shader->GetID(), "uTexture");
				glUniform1i(texLoc, 0);

				GLint uniform_var_color = glGetUniformLocation(shader->GetID(), "uColor");
				glUniform4f(uniform_var_color, 1.f, 1.f, 1.f, 1.f);

				GLint uniform_var_loc1 = glGetUniformLocation(shader->GetID(), "transform");
				if (uniform_var_loc1 >= 0)
					glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mtx.matrix));
				else
					TMT_ERR << "Uniform variable doesn't exist";

				mesh->Draw();
			}
		}
	}
}