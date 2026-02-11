#ifndef TOMATO_SHADER_H
#define TOMATO_SHADER_H

#include <glad/glad.h>

namespace tomato
{
    class Shader
    {
    public:
        Shader(const char* vsName, const char* fsName);
        ~Shader();

        void Use() const;

        //TODO: 유니폼 함수..

    private:
        GLuint programId_{0};
    };
}



#include <string>
#include "glad/glad.h"

class Shader
{
public:
	Shader() = default;
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void Use() const;
	void UnUse() const;

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMat4(const std::string& name, const float* mat) const;

	unsigned int GetID() const { return programID; }

private:
	unsigned int programID = 0;

	void CheckCompileErrors(unsigned int shader, std::string type);
};

#endif //TOMATO_SHADER_H