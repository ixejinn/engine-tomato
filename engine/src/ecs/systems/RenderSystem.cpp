#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tomato/ecs/systems/RenderSystem.h"
#include "tomato/Engine.h"
#include "tomato/ecs/World.h"
#include "tomato/ecs/components/Sprite.h"
#include "tomato/ecs/components/Transform.h"
//#include "tomato/ecs/components/Camera.h"

#include "tomato/ecs/SystemRegistry.h"
REGISTER_SYSTEM(tomato::SystemType::RENDER, RenderSystem)
namespace tomato
{
	RenderSystem::RenderSystem()
	{
		Init();
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

		TMT_LOG << "[shader] " << tag << " is successfully registered. id = " << id;
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

		TMT_LOG << "[texture] " << tag << " is successfully registered. id = " << id;
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

	void RenderSystem::Update(Engine& engine, const SimContext& ctx) { TMT_LOG << "Render Update";  }
	void RenderSystem::Update(const Engine& engine, const SimContext& ctx)
	{
		//TMT_LOG << "const Render Update";

		auto view = engine.GetWorld().View<SpriteComponent, WorldMatrixComponent>();
		for (auto [e, sprite, mtx] : view.each())
		{
			//TMT_LOG << sprite.shader_id << "," << sprite.texture_id;

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

				mesh.Draw();
			}
		}

	//	auto view = world.View<SpriteComponent, WorldMatrixComponent>();
	//	auto camView = world.View<CameraComponent>();
	//	glm::mat4 matView, matProj;
	//	for (auto [e, cam] : camView.each())
	//	{
	//		if (!cam.active) continue;
	//		matView = cam.view;
	//		matProj = cam.projection;
	//	}

	//	for (auto [e, sprite, mtx] : view.each())
	//	{
	//		//TMT_LOG << sprite.shader_id << "," << sprite.texture_id;

	//		Shader* shader = GetShader(sprite.shader_id);
	//		Texture* texture = GetTexture(sprite.texture_id);
	//		//std::cout << "Matrix:" << std::endl;
	//		//for (int i = 0; i < 4; i++) {
	//		//	for (int j = 0; j < 4; j++) {
	//		//		std::cout << mtx.matrix[i][j] << " ";
	//		//	}
	//		//	std::cout << std::endl;
	//		//}
	//		if (texture && shader)
	//		{
	//			shader->Use();
	//			glDisable(GL_DEPTH_TEST);
	//			glad_glActiveTexture(GL_TEXTURE0);
	//			glBindTexture(GL_TEXTURE_2D, texture->GetID());

	//			GLint texLoc = glGetUniformLocation(shader->GetID(), "uTexture");
	//			glUniform1i(texLoc, 0);

	//			GLint uniform_var_color = glGetUniformLocation(shader->GetID(), "uColor");
	//			glUniform4f(uniform_var_color, 1.f, 1.f, 1.f, 1.f);

	//			GLint uniform_var_loc1 = glGetUniformLocation(shader->GetID(), "model");
	//			if (uniform_var_loc1 >= 0)
	//				glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mtx.matrix));
	//			else
	//				TMT_ERR << "Uniform variable doesn't exist";

	//			GLint uniform_var_view = glGetUniformLocation(shader->GetID(), "view");
	//			if (uniform_var_view >= 0)
	//				glUniformMatrix4fv(uniform_var_view, 1, GL_FALSE, glm::value_ptr(matView));
	//			else
	//				TMT_ERR << "Uniform variable doesn't exist";

	//			GLint uniform_var_proj = glGetUniformLocation(shader->GetID(), "projection");
	//			if (uniform_var_proj >= 0)
	//				glUniformMatrix4fv(uniform_var_proj, 1, GL_FALSE, glm::value_ptr(matProj));
	//			else
	//				TMT_ERR << "Uniform variable doesn't exist";

	//			mesh.Draw();
	//		}
	//	}
	}
}