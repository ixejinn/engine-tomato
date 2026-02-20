#ifndef TOMATO_SHADER_H
#define TOMATO_SHADER_H

#include <glad/glad.h>
#include "tomato/tomato_math.h"

namespace tomato
{
    class Shader
    {
    public:
        Shader(const char* vsName, const char* fsName);
        virtual ~Shader();

        void Use() const;

        void SetUniformBool(const char* name, bool value) const;
        void SetUniformInt(const char* name, int value) const;
        void SetUniformFloat(const char* name, float value) const;
        void SetUniformVec4(const char* name, Vector4 value) const;
        void SetUniformMat4(const char* name, Matrix mtx) const;

    private:
        enum Type
        {
            Vertex,
            Fragment,
            Program
        };

        static void CheckCompileErrors(GLuint shader, Type type);

        GLuint programId_{0};
    };
}

#endif //TOMATO_SHADER_H