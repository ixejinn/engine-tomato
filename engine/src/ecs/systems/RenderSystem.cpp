#include "tomato/ecs/systems/RenderSystem.h"
#include "tomato/Engine.h"
#include "tomato/tomato_sim.h"
#include "tomato/services/WindowService.h"
#include "tomato/resource/AssetRegistry.h"
#include "tomato/resource/render/Mesh.h"
#include "tomato/resource/render/Shader.h"
#include "tomato/resource/render/Texture.h"

#include "tomato/ecs/components/Camera.h"
#include "tomato/ecs/components/Tags.h"
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
        // Enable depth test
        glEnable(GL_DEPTH_TEST);

        // Enable color blending and set blend function for alpha transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_CULL_FACE);

        AssetRegistry<Mesh>::GetInstance().Init();
        AssetRegistry<Texture>::GetInstance().Init();
        AssetRegistry<Shader>::GetInstance().Init();
    }

	void RenderSystem::Update(Engine& engine, const SimContext& ctx)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto viewProjection = glm::mat4(1.f);
        CameraComponent* cam{ nullptr };

        auto curCam = engine.GetCurrentCamera();
        if (curCam == entt::null)
        {
            curCam = engine.GetWorld().GetRegistry().view<MainCameraTag>().front();
            
            if (curCam == entt::null)
            {
                TMT_WARN << "Main camera not present";
                return;
            }
            else
                engine.SetCurrentCamera(curCam);
        }
        
        if ((cam = engine.GetWorld().GetRegistry().try_get<CameraComponent>(curCam)))
            viewProjection = cam->viewProjection;
        else
        {
            TMT_WARN << "No camera component in the current camera";
            return;
        }
        

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

            shader->SetUniformMat4("uModel", mtx.matrix);
            shader->SetUniformMat4("uViewProj", viewProjection);
            shader->SetUniformMat3("uNormal", glm::transpose(glm::inverse(glm::mat3(mtx.matrix))));

            shader->SetUniformInt("uTexture", 0);
            shader->SetUniformVec3("uLightPos", glm::vec3(0, 10, 0));
            shader->SetUniformVec4("uColor", render.color);

            mesh->Draw();
        }
    }
}