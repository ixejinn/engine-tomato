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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // struct Vertex::uv
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));

        // Bind element buffer object
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }

    void Mesh::BindVertexArray() const
    {
        glBindVertexArray(vao_);
    }
}

Mesh::Mesh() : VAO(0), VBO(0), EBO(0), indexCount(0)
{
	//나중에 초기화 값 수정할 것
	std::vector<Vertex> _vertices = {
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}}, // 좌하단
		{{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}}, // 우하단
		{{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}}, // 우상단
		{{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}}, // 좌상단
	};

	std::vector<unsigned int> _indices = {
		0, 1, 2,  // 첫 번째 삼각형
		2, 3, 0   // 두 번째 삼각형
	};

	SetupMesh(_vertices, _indices);
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) : VAO(0), VBO(0), EBO(0), indexCount(0)
{
	SetupMesh(vertices, indices);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::BindVAO() const
{
	glBindVertexArray(VAO);
}

void Mesh::Draw() const
{
	BindVAO();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::SetupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	indexCount = static_cast<unsigned int>(indices.size());

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	//EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, uv)));

	glBindVertexArray(0);
}

