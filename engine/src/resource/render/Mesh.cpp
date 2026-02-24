#include "tomato/resource/render/Mesh.h"
#include "tomato/resource/AssetRegistry.h"

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
                vertices = {
                        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}}, // 좌하단
                        {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}}, // 우하단
                        {{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}}, // 우상단
                        {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}}, // 좌상단
                };

                indices = {
                        0, 1, 2,  // 첫 번째 삼각형
                        2, 3, 0   // 두 번째 삼각형
                };
                break;

            case PrimitiveType::CUBE:
                vertices = {
                        // 벽면
                        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
                        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
                        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
                        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},

                        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
                        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
                        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
                        {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},

                        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
                        {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
                        {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
                        {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},

                        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
                        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
                        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
                        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},

                        // 바닥면
                        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
                        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
                        {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
                        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},

                        // 윗면
                        {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f}},
                        {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f}},
                        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
                        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},
                };

                indices = {
                        0, 1, 2,
                        2, 3, 0,

                        4, 5, 6,
                        6, 7, 4,

                        8, 9, 10,
                        10, 11, 8,

                        12, 13, 14,
                        14, 15, 12,

                        16, 17, 18,
                        18, 19, 16,

                        20, 21, 22,
                        22, 23, 20
                };
                break;
        }

        SetMesh(vertices, indices);
    }

    Mesh::~Mesh()
    {
        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(3, &vbo_);
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

    void Mesh::SetMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    {
        vertexCnt_ = (int)indices.size();

        // Generate vertex array object and buffer object names
        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);
        glGenBuffers(1, &ebo_);

        // Bind vertex array object
        glBindVertexArray(vao_);

        // Bind and set buffer object
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        // struct Vertex::position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

        // struct Vertex::uv
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));

        // Bind element buffer object
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }
}
