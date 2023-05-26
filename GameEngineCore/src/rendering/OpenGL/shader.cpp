#include "shader.h"

#include <glad/glad.h>
#include <log.h>

bool GameEngine::Shader::createShader(const char* source, unsigned int type, unsigned int* id)
{
	*id = glCreateShader(type);
	glShaderSource(*id, 1, &source, 0);
	glCompileShader(*id);

	GLint success;
	glGetShaderiv(*id, GL_COMPILE_STATUS, &success);
	if (!success) {
		char info_log[512];
		glGetShaderInfoLog(*id, 512, 0, info_log);
		LOG_ERR("Shader compilation error:\n{}", info_log);
		return false;
	}
	return true;
}

GameEngine::Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource)
{
	m_id = glCreateProgram();
	GLuint vs = 0;
	GLuint fs = 0;

	if (!createShader(vertexShaderSource, GL_VERTEX_SHADER, &vs)) {
		LOG_ERR("Vertex shader compile error");
		glDeleteShader(vs);
		glDeleteShader(fs);
		return;
	}
	if (!createShader(fragmentShaderSource, GL_FRAGMENT_SHADER, &fs)) {
		LOG_ERR("Fragment shader compile error");
		glDeleteShader(vs);
		glDeleteShader(fs);
		return;
	}

	glAttachShader(m_id, vs);
	glAttachShader(m_id, fs);
	glLinkProgram(m_id);

	GLint success;
	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (!success) {
		char info_log[512];
		glGetProgramInfoLog(m_id, 512, 0, info_log);
		LOG_ERR("Shader program compile error:\n{}", info_log);
		
		glDeleteShader(vs);
		glDeleteShader(fs);
		return;
	}

	m_isCompiled = true;
	glDetachShader(m_id, vs);
	glDetachShader(m_id, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

GameEngine::Shader::~Shader()
{
	glDeleteProgram(m_id);
}

void GameEngine::Shader::bind() const
{
	glUseProgram(m_id);
}

void GameEngine::Shader::unbind()
{
	glUseProgram(0);
}

void GameEngine::Shader::setFloat(const char* uniform, float value)
{
	glUniform1f(glGetUniformLocation(m_id, uniform), value);
}

void GameEngine::Shader::setMat4(const char* uniform, const glm::mat4& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, uniform), 1, false, glm::value_ptr(matrix));
}
