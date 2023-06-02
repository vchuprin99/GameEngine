#include "window.h"

#include "rendering/OpenGL/openGL_Renderer.h"
#include "modules/moduleUI.h"

#include <GLFW/glfw3.h>
#include <log.h>

namespace GameEngine {
	Window::Window(uint width, uint height, std::string title)
		: winProps({ width, height, (float)width / (float)height, std::move(title) })
	{
		LOG_INFO("Window created {0}x{1}", width, height);
		int res = init();
		ModuleUI::init(m_window);
	}

	Window::~Window()
	{
		ModuleUI::shutdown();
		shutdown();
	}

	glm::vec2 Window::getCursorPos() const
	{
		double x;
		double y;
		glfwGetCursorPos(m_window, &x, &y);

		return { x, y };
	}

	void Window::enableCursor()
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Window::disableCursor()
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void Window::setCursorPos(const glm::vec2& pos)
	{
		glfwSetCursorPos(m_window, pos.x, pos.y);
	}

	int Window::init()
	{
		glfwSetErrorCallback([](int error_code, const char* description) {
			LOG_ERR("GLFW error: {}", description);
		});

		if (!glfwInit()) {
			LOG_CRIT("GLFW initialization failure");
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(winProps.width, winProps.height, winProps.title.c_str(), NULL, NULL);
		if (!m_window)
		{
			LOG_CRIT("Window creation failure");
			return -2;
		}
		if (!OpenGL_Renderer::init(m_window)) {
			LOG_CRIT("OpenGL renderer initialization failure");
			return -3;
		}

		LOG_INFO("OpenGL version - {}", OpenGL_Renderer::getVersion());
		LOG_INFO("Renderer - {}", OpenGL_Renderer::getRenderer());

		glfwSetWindowUserPointer(m_window, &winProps);

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
			WinProps* winProps = reinterpret_cast<WinProps*>(glfwGetWindowUserPointer(window));
			winProps->width = width;
			winProps->height = height;
			winProps->aspect_ratio = (float)width / (float)height;

			WindowResizeEvent event(width, height);
			winProps->eventCallback(event);
			});
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			WinProps* winProps = reinterpret_cast<WinProps*>(glfwGetWindowUserPointer(window));

			WindowCloseEvent event;
			winProps->eventCallback(event);
			});
		glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
			OpenGL_Renderer::setViewPort(width, height);
			});
		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WinProps* winProps = reinterpret_cast<WinProps*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS: {
				KeyPressedEvent event(static_cast<KeyCode>(key), false);
				winProps->eventCallback(event);
				break;
			}
			case GLFW_RELEASE: {
				KeyReleasedEvent event(static_cast<KeyCode>(key));
				winProps->eventCallback(event);
				break;
			}
			case GLFW_REPEAT: {
				KeyPressedEvent event(static_cast<KeyCode>(key), true);
				winProps->eventCallback(event);
				break;
			}
			}
			});
		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
			WinProps* winProps = reinterpret_cast<WinProps*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event(xpos, ypos);
			winProps->eventCallback(event);
			});
		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
			WinProps* winProps = reinterpret_cast<WinProps*>(glfwGetWindowUserPointer(window));

			MouseScrolledEvent event(xoffset);
			winProps->eventCallback(event);
			});
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) 
		{
			WinProps* winProps = reinterpret_cast<WinProps*>(glfwGetWindowUserPointer(window));

			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			switch (action)
			{
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(static_cast<MouseButton>(button), xpos, ypos);
					winProps->eventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(static_cast<MouseButton>(button), xpos, ypos);
					winProps->eventCallback(event);
					break;
				}
			}
		}
		);

		return 0;
	}



	void Window::onUpdate()
	{
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	void Window::shutdown()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}