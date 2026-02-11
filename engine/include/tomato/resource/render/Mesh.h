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
        //Mesh(const char* filename); TODO: 파일 읽어서 메시 생성할 수 있도록 추가
        ~Mesh();

        void Bind() const;
        void Draw() const;

    private:
        void SetMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        /// Stores the state related to vertex attribute settings.
        GLuint vao_{0};

        /// Stores the actual vertex data (positions, texture coordinates, ...)
        GLuint vbo_{0};

        /// Stores the indices used for element drawing. Indices array
        GLuint ebo_{0};

        GLuint indicesCount_{0};
    };
}

#endif //TOMATO_MESH_H