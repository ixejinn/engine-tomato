#include "tomato/ecs/systems/RenderSystem.h"
#include "tomato/Engine.h"
#include "tomato/tomato_sim.h"
#include "tomato/services/WindowService.h"
#include "tomato/resource/AssetRegistry.h"
#include "tomato/resource/render/Mesh.h"
#include "tomato/resource/render/Shader.h"
#include "tomato/resource/render/Texture.h"

#include "tomato/ecs/components/Render.h"
#include "tomato/ecs/components/Transform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "tomato/RegistryEntry.h"
REGISTER_SYSTEM(tomato::SystemPhase::RENDER, RenderSystem)

namespace tomato
{
    RenderSystem::RenderSystem() :
    curMesh_(GetAssetID(Mesh::GetName(Mesh::PrimitiveType::PLAIN))),
    curShader_(GetAssetID(Shader::PrimitiveName)),
    curTexture_(GetAssetID(Texture::PrimitiveName))
    {
        glEnable(GL_DEPTH_TEST);

        AssetRegistry<Mesh>::GetInstance().Init();
        AssetRegistry<Texture>::GetInstance().Init();
        AssetRegistry<Shader>::GetInstance().Init();
    }

	void RenderSystem::Update(Engine& engine, const SimContext& ctx)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO: 임시 카메라 수정
        // 카메라의 엔티티ID를 가지고 있게 하든지..
        auto view = glm::mat4(1.f);
        view = glm::translate(view, glm::vec3(0.f, 0.f, -10.f));    // 카메라 위치만큼 옮기기

        auto projection = glm::mat4(1.f);
        //           glm::perspective(fovy, aspect, zNear, zFar)
        auto& window = engine.GetWindow();
        projection = glm::perspective(glm::radians(45.f), (float)window.GetWidth() / window.GetHeight(), 0.1f, 100.f);
        //projection = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 100.f);
        // fovy   : field of view in the y direction 시야각(카메라 렌즈의 각도)
        // aspect : 화면의 가로 너비 / 세로 높이 비율
        // zNear  : frustum의 근평면 (물체가 이 거리보다 멀어야 그려짐)
        // zFar   : frustum의 원평면 (물체가 이 거리보다 가까워야 그려짐)

        // TODO: frustum culling 위한 Frustum 클래스? 필요하다면 만들기 (6개 면과 엔티티 위치 비교)

        auto viewProjection = projection * view;

        Mesh* mesh = AssetRegistry<Mesh>::GetInstance().Get(curMesh_);
        mesh->Bind();

        Shader* shader = AssetRegistry<Shader>::GetInstance().Get(curShader_);
        shader->Use();

        AssetRegistry<Texture>::GetInstance().Get(curTexture_)->Bind();

        auto group = engine.GetWorld().GetRegistry().group<PositionComponent, RenderComponent, WorldMatrixComponent>();
        for (auto [e, pos, render, mtx] : group.each())
        {
            // TODO: frustum culling

            if (curShader_ != render.shader)
            {
                curShader_ = render.shader;
                shader = AssetRegistry<Shader>::GetInstance().Get(curShader_);
                shader->Use();
            }

            if (curTexture_ != render.texture)
            {
                curTexture_ = render.texture;
                AssetRegistry<Texture>::GetInstance().Get(curTexture_)->Bind();
            }

            if (curMesh_ != render.mesh)
            {
                curMesh_ = render.mesh;
                mesh = AssetRegistry<Mesh>::GetInstance().Get(curMesh_);
                mesh->Bind();
            }

            shader->SetUniformInt("uTexture", 0);
            shader->SetUniformVec4("uColor", render.color);
            shader->SetUniformMat4("uModel", mtx.matrix);
            shader->SetUniformMat4("uViewProj", viewProjection);

            mesh->Draw();
        }
    }
}