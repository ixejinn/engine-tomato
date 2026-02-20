#include "tomato/ecs/systems/RenderSystem.h"
#include "tomato/ecs/components/Transform.h"
#include "tomato/ecs/components/Render.h"
#include "tomato/resource/AssetRegistry.h"
#include "tomato/resource/render/Mesh.h"
#include "tomato/resource/render/Shader.h"
#include "tomato/resource/render/Texture.h"
#include "tomato/ecs/World.h"
#include "tomato/Engine.h"
#include "tomato/Logger.h"
#include "tomato/services/WindowService.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "tomato/ecs/SystemRegistry.h"
REGISTER_SYSTEM(tomato::SystemPhase::RENDER, RenderSystem)

namespace tomato
{
    RenderSystem::RenderSystem()
    {
        // 기본 에셋 추가

        glEnable(GL_DEPTH_TEST);
    }

    void RenderSystem::Update(Engine& engine, const SimContext& ctx)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static auto group = engine.GetWorld().GetRegistry().group<PositionComponent, WorldMatrixComponent, RenderComponent>();
        // TODO: group 정렬
        //group.sort<> 정렬 조건: 불투명 셰이더(가까운 → 먼) → 투명 셰이더(먼 → 가까운)

        Shader* shader = AssetRegistry<Shader>::GetInstance().Get(curShader_);

        // TODO: 임시 카메라 수정
        // 카메라의 엔티티ID를 가지고 있게 하든지..
        auto view = glm::mat4(1.f);
        view = glm::translate(view, glm::vec3(0.f, 0.f, -10.f));    // 카메라 위치만큼 옮기기

        auto projection = glm::mat4(1.f);
        //           glm::perspective(fovy, aspect, zNear, zFar)
        auto& window = engine.GetWindow();
        projection = glm::perspective(glm::radians(45.f), (float)window.GetWidth() / window.GetHeight(), 0.1f, 100.f);
        // fovy   : field of view in the y direction 시야각(카메라 렌즈의 각도)
        // aspect : 화면의 가로 너비 / 세로 높이 비율
        // zNear  : frustum의 근평면 (물체가 이 거리보다 멀어야 그려짐)
        // zFar   : frustum의 원평면 (물체가 이 거리보다 가까워야 그려짐)

        // TODO: frustum culling 위해서 Frustum 클래스 만드는게 나을듯 6개 면과 엔티티 비교할 수 있게

        auto viewProjection = projection * view;

        for (auto [e, pos, mtx, render] : group.each())
        {
            if (curShader_ != render.shader)
            {
                curShader_ = render.shader;
                shader = AssetRegistry<Shader>::GetInstance().Get(curShader_);
                shader->Use();
            }

            // TODO: frustum culling

            shader->SetUniformMat4("uModel", mtx.matrix);
            shader->SetUniformMat4("uViewProj", viewProjection);
            shader->SetUniformVec4("uColor", render.color);


            if (curMesh_ != render.mesh)
            {
                AssetRegistry<Mesh>::GetInstance().Get(render.mesh)->Bind();
                curMesh_ = render.mesh;
            }


        }
    }
}


#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tomato/ecs/systems/RenderSystem.h"
#include "tomato/Engine.h"
#include "tomato/tomato_sim.h"
#include "tomato/ecs/components/Render.h"
#include "tomato/ecs/components/Transform.h"
//#include "tomato/ecs/components/Camera.h"

#include "tomato/ecs/SystemRegistry.h"
REGISTER_SYSTEM(tomato::SystemPhase::RENDER, RenderSystem)
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

//        //나중에 초기화 값 수정할 것
//        std::vector<Vertex> _vertices = {
//                {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}}, // 좌하단
//                {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}}, // 우하단
//                {{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}}, // 우상단
//                {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}}, // 좌상단
//        };
//
//        std::vector<unsigned int> _indices = {
//                0, 1, 2,  // 첫 번째 삼각형
//                2, 3, 0   // 두 번째 삼각형
//        };
	}

	void RenderSystem::Update(Engine& engine, const SimContext& ctx) { TMT_LOG << "Render Update";  }
	void RenderSystem::Update(const Engine& engine, const SimContext& ctx)
	{
		//TMT_LOG << "const Render Update";

		auto view = engine.GetWorld().GetRegistry().view<SpriteComponent, WorldMatrixComponent>();
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

	//	for (auto [e, sprite, mtx_] : view.each())
	//	{
	//		//TMT_LOG << sprite.shader_id << "," << sprite.texture_id;

	//		Shader* shader = GetShader(sprite.shader_id);
	//		Texture* texture = GetTexture(sprite.texture_id);
	//		//std::cout << "Matrix:" << std::endl;
	//		//for (int i = 0; i < 4; i++) {
	//		//	for (int j = 0; j < 4; j++) {
	//		//		std::cout << mtx_.matrix[i][j] << " ";
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
	//				glUniformMatrix4fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(mtx_.matrix));
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