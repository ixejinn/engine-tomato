#ifndef TOMATO_SHADER_H
#define TOMATO_SHADER_H

#include <glad/glad.h>
#include "tomato/tomato_math.h"

namespace tomato
{
    class Shader
    {
    public:
        constexpr static const char* PrimitiveName = "Shader_PRIMITIVE";

        static void Create();
        static void Create(const char* vsName, const char* fsName);

    private:
        Shader();
        Shader(const char* vsName, const char* fsName);

    public:
        ~Shader();

        void Use() const;

        void SetUniformBool(const char* name, bool value) const;
        void SetUniformInt(const char* name, int value) const;
        void SetUniformFloat(const char* name, float value) const;


        void SetUniformVec3(const char* name, float v0, float v1, float v2) const;
        void SetUniformVec3(const char* name, Vector3 value) const;
        void SetUniformVec4(const char* name, float v0, float v1, float v2, float v3) const;
        void SetUniformVec4(const char* name, Vector4 value) const;

        void SetUniformMat3(const char* name, Matrix3 value) const;
        void SetUniformMat4(const char* name, Matrix4 value) const;

    private:
        enum Type
        {
            VERTEX,
            FRAGMENT,
            PROGRAM
        };

        static void CheckCompileErrors(GLuint shader, Type type);

        void ReadShaderSourceFile(const char* vsName, const char* fsName);

        GLuint programId_{0};
    };
}

#endif //TOMATO_SHADER_H