#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>

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

#endif