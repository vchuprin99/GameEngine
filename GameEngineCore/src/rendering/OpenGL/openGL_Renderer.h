#pragma once

struct GLFWwindow;

namespace GameEngine {
	class VertexArray;

	class OpenGL_Renderer {
	public:
		static bool init(GLFWwindow* pWindow);

		static void draw(const VertexArray& vertexArray);
		static void setClearColor(const float r, const float g, const float b, const float a);
		static void clear();
		static void setViewPort(const int width, const int height, const int bottomOffset = 0, const int leftOffset = 0);

		static const char* getVersion();
		static const char* getRenderer();
		static const char* getVendor();
	};
}