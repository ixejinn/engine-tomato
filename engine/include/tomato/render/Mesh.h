#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
};

class Mesh
{
public:
	Mesh();
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	~Mesh();


	void BindVAO() const;
	void Draw() const;
	void SetupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

private:
	unsigned int VAO, VBO, EBO;
	unsigned int indexCount;

};

#endif