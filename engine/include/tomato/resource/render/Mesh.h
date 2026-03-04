#ifndef TOMATO_MESH_H
#define TOMATO_MESH_H

#include <vector>
#include <memory>
#include <glad/glad.h>
#include "tomato/tomato_math.h"

namespace tomato
{
    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector2 uv;

        Vertex() = default;
        Vertex(Vector3 pos, Vector3 n, Vector2 uv) : position(pos), normal(n), uv(uv) {}
    };

    class Mesh
    {
    public:
        enum class PrimitiveType
        {
            PLAIN,
            CUBE,
            COUNT
        };

        constexpr static const char* GetName(PrimitiveType type)
        {
            switch (type)
            {
                case PrimitiveType::PLAIN:
                    return "Mesh_PLAIN";
                case PrimitiveType::CUBE:
                default:
                    return "Mesh_CUBE";
            }
        }

        static void Create();
        //static void Create(const char* filename);

    private:
        Mesh(PrimitiveType type);
        //Mesh(const char* filename);

    public:
        ~Mesh();

        void Bind() const;
        void Draw() const;

    private:
        /**
        * @brief Sets CCW vertex list of Plain
        * 
        * v0---v3
        * |  / |
        * | /  |
        * v1---v2
        */
        static void Plain(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
                          std::vector<Vertex>& vertices, size_t vOffset,
                          std::vector<unsigned int>& indices, size_t iOffset);
        void SetMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

        /// Stores the state related to vertex attribute settings.
        GLuint vao_{0};

        /// Stores the actual vertex data (positions, texture coordinates, ...)
        GLuint vbo_{0};

        /// Stores the indices used for element drawing. Indices array.
        GLuint ebo_{0};

        int vertexCnt_{0};
    };
}

#endif //TOMATO_MESH_H