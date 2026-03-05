#include "tomato/resource/render/Mesh.h"
#include "tomato/resource/AssetRegistry.h"

#include <glm/glm.hpp>

namespace tomato
{
    void Mesh::Create()
    {
        auto& registry = AssetRegistry<Mesh>::GetInstance();
        for (int i = 0; i < static_cast<int>(PrimitiveType::COUNT); ++i)
        {
            auto type = static_cast<PrimitiveType>(i);

            std::unique_ptr<Mesh> ptr{new Mesh(type)};
            registry.Register(GetName(type), std::move(ptr));
        }
    }

    Mesh::Mesh(PrimitiveType type)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        switch (type)
        {
            case PrimitiveType::PLAIN:
            {
                vertices.resize(4 * 1);
                indices.resize(3 * 2 * 1);

                glm::vec3 v0{-0.5f,  0.5f, 0.0f};
                glm::vec3 v1{-0.5f, -0.5f, 0.0f};
                glm::vec3 v2{ 0.5f, -0.5f, 0.0f};
                glm::vec3 v3{ 0.5f,  0.5f, 0.0f};

                Plain(v0, v1, v2, v3, vertices, 0, indices, 0);
            }
                break;
            case PrimitiveType::CUBE:
            {
                vertices.resize(4 * 6);
                indices.resize(3 * 2 * 6);

                glm::vec3 v0{-0.5f,  0.5f, -0.5f};
                glm::vec3 v1{-0.5f,  0.5f,  0.5f};
                glm::vec3 v2{ 0.5f,  0.5f,  0.5f};
                glm::vec3 v3{ 0.5f,  0.5f, -0.5f};

                glm::vec3 v4{-0.5f, -0.5f, -0.5f};
                glm::vec3 v5{-0.5f, -0.5f,  0.5f};
                glm::vec3 v6{ 0.5f, -0.5f,  0.5f};
                glm::vec3 v7{ 0.5f, -0.5f, -0.5f};

                Plain(v0, v1, v2, v3, vertices, 4 * 0, indices, 3 * 2 * 0); // top
                Plain(v1, v5, v6, v2, vertices, 4 * 1, indices, 3 * 2 * 1);
                Plain(v2, v6, v7, v3, vertices, 4 * 2, indices, 3 * 2 * 2);
                Plain(v3, v7, v4, v0, vertices, 4 * 3, indices, 3 * 2 * 3);
                Plain(v0, v4, v5, v1, vertices, 4 * 4, indices, 3 * 2 * 4);
                Plain(v5, v4, v7, v6, vertices, 4 * 5, indices, 3 * 2 * 5); // bottom
            }
                break;
            case PrimitiveType::COUNT:
                TMT_WARN << "Invalid primitive type";
                break;
        }

        SetMesh(vertices, indices);
    }

    Mesh::~Mesh()
    {
        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(1, &vbo_);
        glDeleteBuffers(1, &ebo_);
    }

    void Mesh::Bind() const
    {
        glBindVertexArray(vao_);
    }

    void Mesh::Draw() const
    {
        glDrawElements(GL_TRIANGLES, vertexCnt_, GL_UNSIGNED_INT, nullptr);
    }

    void Mesh::Plain(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
        std::vector<Vertex>& vertices, const size_t vOffset,
        std::vector<unsigned int>& indices, const size_t iOffset)
    {
        const glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
        
        vertices[vOffset + 0] = Vertex(v0, normal, { 0.0f, 1.0f });
        vertices[vOffset + 1] = Vertex(v1, normal, { 0.0f, 0.0f });
        vertices[vOffset + 2] = Vertex(v2, normal, { 1.0f, 0.0f });
        vertices[vOffset + 3] = Vertex(v3, normal, { 1.0f, 1.0f });

        indices[iOffset + 0] = vOffset + 0;    // triangle v0 → v1 → v3
        indices[iOffset + 1] = vOffset + 1;
        indices[iOffset + 2] = vOffset + 3;
        indices[iOffset + 3] = vOffset + 3;    // triangle v3 → v1 → v2
        indices[iOffset + 4] = vOffset + 1;
        indices[iOffset + 5] = vOffset + 2;
    }

    void Mesh::SetMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    {
        vertexCnt_ = static_cast<int>(indices.size());

        // Generate vertex array object and buffer objects
        glCreateVertexArrays(1, &vao_);
        glCreateBuffers(1, &vbo_);
        glCreateBuffers(1, &ebo_);

        // Upload vertex data to VBO
        glNamedBufferData(vbo_, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);
        // Bind VBO to vertex binding slot 0 of VAO
        glVertexArrayVertexBuffer(vao_, 0, vbo_, 0, sizeof(Vertex));

        // Vertex attribute 0: Vertex::position
        // Enable attribute slot in shader
        glEnableVertexArrayAttrib(vao_, 0);
        // Link attribute 0 to vertex binding slot 0 (where VBO is bound)
        glVertexArrayAttribBinding(vao_, 0, 0);
        // Define the data layout (starting at Vertex::position offset)
        glVertexArrayAttribFormat(vao_, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));

        // Vertex attribute 1: Vertex::normal
        glEnableVertexArrayAttrib(vao_, 1);
        glVertexArrayAttribBinding(vao_, 1, 0);
        glVertexArrayAttribFormat(vao_, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));

        // Vertex attribute 2: Vertex::uv
        glEnableVertexArrayAttrib(vao_, 2);
        glVertexArrayAttribBinding(vao_, 2, 0);
        glVertexArrayAttribFormat(vao_, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));

        // Upload index(element) data to EBO
        glNamedBufferData(ebo_, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), indices.data(), GL_STATIC_DRAW);
        // Bind EBO to element binding slot of VAO
        glVertexArrayElementBuffer(vao_, ebo_);
    }
}
