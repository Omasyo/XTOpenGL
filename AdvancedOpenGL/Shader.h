#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Shader
{
public:
	GLuint ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, GLint value) const;
	void setFloat(const std::string& name, GLfloat value) const;
	void setVec2(const std::string& name, const glm::vec2& value) const;
	void setVec2(const std::string& name, GLfloat x, GLfloat y) const;
	void setVec3(const std::string& name, const glm::vec3& value) const;
	void setVec3(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const;
	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
	void checkCompileErrors(GLuint shader, std::string type);
};

