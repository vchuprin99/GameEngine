#include "openGL_Renderer.h"

#include "vertexArray.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <log.h>

bool GameEngine::OpenGL_Renderer::init(GLFWwindow* pWindow)
{
	glfwMakeContextCurrent(pWindow);

	if (!gladLoadGL()) {
		LOG_CRIT("GLAD load failure");
		return false;
	}
	LOG_INFO("GLAD initialized");

	return true;
}

void GameEngine::OpenGL_Renderer::draw(const VertexArray& vertexArray)
{
	glDrawElements(GL_TRIANGLES, vertexArray.getIndicesCount(), GL_UNSIGNED_INT, nullptr);
}

void GameEngine::OpenGL_Renderer::setClearColor(const float r, const float g, const float b, const float a)
{
	glClearColor(r, g, b, a);
}

void GameEngine::OpenGL_Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void GameEngine::OpenGL_Renderer::setViewPort(const int width, const int height, const int bottomOffset, const int leftOffset)
{
	glViewport(bottomOffset, leftOffset, width, height);
}

const char* GameEngine::OpenGL_Renderer::getVersion()
{
	return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

const char* GameEngine::OpenGL_Renderer::getRenderer()
{
	return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

const char* GameEngine::OpenGL_Renderer::getVendor()
{
	return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
}
