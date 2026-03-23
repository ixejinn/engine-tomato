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
            SPHERE,
            CYLINDER,
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
                case PrimitiveType::SPHERE:
                    return "Mesh_SPHERE";
                case PrimitiveType::CYLINDER:
                    return "Mesh_Cylinder";
            }
        }

        /**
         * @brief Generates primitive meshes and registers them to the asset registry.
         */
        static void Create();
        //static void Create(const char* filename);

    private:
        explicit Mesh(PrimitiveType type);
        //Mesh(const char* filename);

    public:
        ~Mesh();

        void Bind() const;
        void Draw(bool drawLine = false) const;

    private:
        static void Plain(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
        static void Cube(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
        static void Sphere(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
        static void Cylinder(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

        /**
         * @brief Populates vertex and index buffers for a single face (quad).
         * @param vertices Reference to the vertex vector to ve populated.
         * @param indices Reference to the index vector to be populated.
         *
         * v0---v3
         * |  / |
         * | /  |
         * v1---v2
         */
        static void FillMeshData(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3,
                                 std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

        /**
         * @brief Populates vertex and index buffers for a single face (triangle).
         * @param vertices Reference to the vertex vector to ve populated.
         * @param indices Reference to the index vector to be populated.
         *
         *    v0
         *   /  \
         * v1---v2
         */
        static void FillMeshData(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2,
                                 std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

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