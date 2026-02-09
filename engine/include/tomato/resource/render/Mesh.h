#ifndef TOMATO_MESH_H
#define TOMATO_MESH_H

#include <vector>
#include <glad/glad.h>
#include "tomato/tomato_math.h"

namespace tomato
{
    struct Vertex
    {
        Vector3 position;
        Vector2 uv;
    };

    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        ~Mesh();

        void SetMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        void BindVertexArray() const;

    private:
        /// Stores the state related to vertex attribute settings.
        GLuint vao_{0};

        /// Stores the actual vertex data (positions, texture coordinates, ...)
        GLuint vbo_{0};

        /// Stores the indices used for element drawing. Indices array
        GLuint ebo_{0};

        GLuint indicesCount_{0};
    };
}

//#include "glm/vec2.hpp"
//#include "glm/vec3.hpp"
//#include "glm/mat4x4.hpp"
//#include "glad/glad.h"

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