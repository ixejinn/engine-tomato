#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tomato/ecs/systems/RenderSystem.h"
#include "tomato/Engine.h"
#include "tomato/tomato_sim.h"
#include "tomato/ecs/components/Sprite.h"
#include "tomato/ecs/components/Transform.h"
//#include "tomato/ecs/components/Camera.h"
#include "tomato/services/WindowService.h"

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//TMT_INFO << "const Render Update";

        // TODO: 임시 카메라 수정
        // 카메라의 엔티티ID를 가지고 있게 하든지..
        auto view = glm::mat4(1.f);
        view = glm::translate(view, glm::vec3(0.f, 0.f, -10.f));    // 카메라 위치만큼 옮기기

        auto projection = glm::mat4(1.f);
        //           glm::perspective(fovy, aspect, zNear, zFar)
        auto& window = engine.GetWindow();
        projection = glm::perspective(glm::radians(45.f), (float)window.GetWidth() / window.GetHeight(), 0.1f, 100.f);
        //TMT_INFO << window.GetWidth() << ", " << window.GetHeight() << (float)window.GetWidth() / window.GetHeight();
        //projection = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 100.f);
        // fovy   : field of view in the y direction 시야각(카메라 렌즈의 각도)
        // aspect : 화면의 가로 너비 / 세로 높이 비율
        // zNear  : frustum의 근평면 (물체가 이 거리보다 멀어야 그려짐)
        // zFar   : frustum의 원평면 (물체가 이 거리보다 가까워야 그려짐)

        auto viewProjection = projection * view;

		auto group = engine.GetWorld().GetRegistry().view<SpriteComponent, WorldMatrixComponent>();
		for (auto [e, sprite, mtx] : group.each())
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

                shader->SetUniformInt("uTexture", 0);
                shader->SetUniformVec4("uColor", 1.f, 1.f, 1.f, 1.f);
                shader->SetUniformMat4("uModel", mtx.matrix);
                shader->SetUniformMat4("uViewProj", viewProjection);

				mesh->Draw();
			}
		}
	}
}