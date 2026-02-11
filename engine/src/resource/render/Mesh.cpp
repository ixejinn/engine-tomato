#include "tomato/resource/render/Mesh.h"

namespace tomato
{
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    {
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
        glDrawElements(GL_TRIANGLES, indicesCount_, GL_UNSIGNED_INT, nullptr);
    }

    void Mesh::SetMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    {
        indicesCount_ = indices.size();

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