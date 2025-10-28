#include "tomato/render/Mesh.h"
#include <iostream>

Mesh::Mesh() : VAO(0), VBO(0), EBO(0), indexCount(0)
{
	//���߿� �ʱ�ȭ �� ������ ��
	std::vector<Vertex> _vertices = {
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}}, // ���ϴ�
		{{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}}, // ���ϴ�
		{{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}}, // ����
		{{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}}, // �»��
	};

	std::vector<unsigned int> _indices = {
		0, 1, 2,  // ù ��° �ﰢ��
		2, 3, 0   // �� ��° �ﰢ��
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

