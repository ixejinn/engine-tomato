#include "tomato/resource/render/Shader.h"
#include "tomato/Logger.h"
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

namespace tomato
{
    Shader::Shader(const char* vsName, const char* fsName)
    {
        // READ SHADER SOURCE FILE
        // For vertex shader
        std::ifstream vsFile;
        std::string vsSrcStr;

        // For fragment shader
        std::ifstream fsFile;
        std::string fsSrcStr;

        vsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fsFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            // Open files
            vsFile.open(vsName);
            fsFile.open(fsName);

            // Read file's buffer contents into stream
            std::stringstream vsStream, fsStream;
            vsStream << vsFile.rdbuf();
            fsStream << fsFile.rdbuf();

            // Close file handlers
            vsFile.close();
            fsFile.close();

            // Convert stream into string
            vsSrcStr = vsStream.str();
            fsSrcStr = fsStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            TMT_ERR << "Failed to open file or read error: " << e.what();
        }

        const char* vsSrc = vsSrcStr.c_str();
        const char* fsSrc = fsSrcStr.c_str();

        // COMPILE SHADERS
        // Vertex shader
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vsSrc, nullptr);
        glCompileShader(vs);
        CheckCompileErrors(vs, Vertex);

        // Fragment shader
        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fsSrc, nullptr);
        glCompileShader(fs);
        CheckCompileErrors(fs, Fragment);

        // Link shaders
        programId_ = glCreateProgram();
        glAttachShader(programId_, vs);
        glAttachShader(programId_, fs);
        glLinkProgram(programId_);
        CheckCompileErrors(programId_, Program);

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    Shader::~Shader()
    {
        if (programId_)
            glDeleteProgram(programId_);
    }

    void Shader::Use() const
    {
        glUseProgram(programId_);
    }

    void Shader::SetUniformBool(const char* name, bool value) const
    {
        SetUniformInt(name, (int)value);
    }

    void Shader::SetUniformInt(const char* name, int value) const
    {
        glUniform1i(
            glGetUniformLocation(programId_, name),
            value
        );
    }

    void Shader::SetUniformFloat(const char* name, float value) const
    {
        glUniform1f(
            glGetUniformLocation(programId_, name),
            value
        );
    }

    void Shader::SetUniformVec4(const char* name, Vector4 value) const
    {
        glUniform4fv(
                glGetUniformLocation(programId_, name),
                4,
                glm::value_ptr(value)
        );
    }

    void Shader::SetUniformMat4(const char* name, Matrix value) const
    {
        glUniformMatrix4fv(
            glGetUniformLocation(programId_, name),
            1,
            GL_FALSE,
            glm::value_ptr(value)
        );
    }

    void Shader::CheckCompileErrors(GLuint shader, Type type)
    {
        int success;
        char infoLog[512];

        switch (type)
        {
            case Vertex:
            case Fragment:
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
                    TMT_ERR << (type == Vertex ? "Vertex" : "Fragment") << " shader compile error\n"
                            << infoLog << "\n -- --------------------------------------------------- --\n";
                }
                break;

            case Program:
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shader, 512, nullptr, infoLog);
                    TMT_ERR << "Program link error\n"
                            << infoLog << "\n -- --------------------------------------------------- --\n";
                }
                break;
        }
    }
}