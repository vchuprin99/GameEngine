#include "window.h"

#include "rendering/OpenGL/shader.h"
#include "rendering/OpenGL/vertexBuffer.h"
#include "rendering/OpenGL/vertexArray.h"
#include "rendering/OpenGL/indexBuffer.h"
#include "rendering/camera.h"
#include "rendering/OpenGL/openGL_Renderer.h"
#include "modules/moduleUI.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <GLFW/glfw3.h>
#include <log.h>

#include <glm/mat4x4.hpp>

namespace GameEngine {
	std::unique_ptr<Shader> shader;
	std::unique_ptr<VertexArray> vertexArray;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;

	float scale[3] = { 0.5, 0.5, 0.5 };
	float rotate = 0;

	float camera_position[3] = { 0 };
	float camera_rotation[3] = { 0 };
	bool isPerspectiveMode = true;
	Camera camera(
		glm::vec3(camera_position[0], camera_position[1], camera_position[2]),
		glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2])
	);

	glm::mat4 scale_matrix = {
		scale[0], 0, 0, 0,
		0, scale[1], 0, 0,
		0, 0, scale[2], 0,
		0, 0, 0, 1
	};
	glm::mat4 rotate_matrix = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	glm::mat4 model_matrix = rotate_matrix * scale_matrix;

	float points[] = {
		 0.5,  0.5, -6,      1, 0, 0,
		 0.5, -0.5, -6,      0, 1, 0,
		-0.5,  0.5, -6,      0, 0, 1,
		-0.5, -0.5, -6,      0, 1, 1,

		 0.5,  0.5, -5,      1, 0, 0,
		 0.5, -0.5, -5,      0, 1, 0,
		-0.5,  0.5, -5,      0, 0, 1,
		-0.5, -0.5, -5,      0, 1, 1
	};
	GLuint indices[] = { 0, 1, 2, 1, 2, 3, 4, 5, 6, 5, 6, 7 };

	const char* vertexShader = R"(
        #version 460

        layout (location = 0) in vec3 pos;
        layout (location = 1) in vec3 color;

        out vec4 vertexColor;

		uniform float aspect_ratio;
		uniform mat4 model_matrix;
		uniform mat4 view_matrix;
		uniform mat4 projection_matrix;

        void main(){
			gl_Position = projection_matrix * view_matrix * model_matrix * vec4(pos, 1);
			gl_Position.y *= aspect_ratio; 
            vertexColor = vec4(color, 1);
        }
    )";
	const char* fragmentShader = R"(
        #version 460
        
        in vec4 vertexColor;
        out vec4 fragmentColor;

        void main(){
            fragmentColor = vertexColor;
        }        
    )";
	Window::Window(uint width, uint height, std::string title)
		: winProps({ width, height, (float)width / (float)height, std::move(title) })
	{
		LOG_INFO("Window created {0}x{1}", width, height);
		int res = init();
	}

	Window::~Window()
	{
		ModuleUI::shutdown();
		shutdown();
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
				KeyPressedEvent event(key, false);
				winProps->eventCallback(event);
				break;
			}
			case GLFW_RELEASE: {
				KeyReleasedEvent event(key);
				winProps->eventCallback(event);
				break;
			}
			case GLFW_REPEAT: {
				KeyPressedEvent event(key, true);
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
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			WinProps* winProps = reinterpret_cast<WinProps*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS: {
				MouseButtonPressedEvent event(button);
				winProps->eventCallback(event);
				break;
			}
			case GLFW_RELEASE: {
				MouseButtonReleasedEvent event(button);
				winProps->eventCallback(event);
				break;
			}
			}
			});

		ModuleUI::init(m_window);

		BufferLayout bufferLayout{
			ShaderDataType::Float3,
			ShaderDataType::Float3
		};
		vertexArray = std::make_unique<VertexArray>();
		vertexBuffer = std::make_unique<VertexBuffer>(points, sizeof(points), bufferLayout);

		indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

		vertexArray->addVertexBuffer(*vertexBuffer);
		vertexArray->setIndexBuffer(*indexBuffer);

		shader = std::make_unique<Shader>(vertexShader, fragmentShader);

		return 0;
	}



	void Window::on_update()
	{
		shader->bind();
		indexBuffer->bind();
		shader->setFloat("aspect_ratio", getAspect());
		shader->setMat4("model_matrix", model_matrix);

		OpenGL_Renderer::clear();

		scale_matrix = {
			scale[0], 0, 0, 0,
			0, scale[1], 0, 0,
			0, 0, scale[2], 0,
			0, 0, 0, 1
		};
		float rotateRadians = -glm::radians(rotate);
		rotate_matrix = {
			cos(rotateRadians), sin(rotateRadians), 0, 0,
			-sin(rotateRadians), cos(rotateRadians), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		model_matrix = rotate_matrix * scale_matrix;

		camera.setPositionRotation(
			glm::vec3(camera_position[0], camera_position[1], camera_position[2]),
			glm::vec3(camera_rotation[0], camera_rotation[1], camera_rotation[2])
		);
		camera.setProjectionMode(
			isPerspectiveMode ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic
		);
		shader->setMat4("view_matrix", camera.getViewMatrix());
		shader->setMat4("projection_matrix", camera.getProjectionMatrix());

		OpenGL_Renderer::draw(*vertexArray);

		ModuleUI::updateBegin();

		

		ImGui::Begin("Properties");

		ImGui::SliderFloat3("Scale", scale, -5, 5);
		ImGui::SliderFloat("Rotation", &rotate, 0, 360);
		ImGui::SliderFloat3("Camera Position", camera_position, -5, 5);
		ImGui::SliderFloat3("Camera Rotation", camera_rotation, 0, 360);
		ImGui::Checkbox("Perspective mode", &isPerspectiveMode);
		if (ImGui::Button("Default")) {
			scale[0] = 0.5;
			scale[1] = 0.5;
			scale[2] = 0.5;
			rotate = 0;

			camera_position[0] = 0;
			camera_position[1] = 0;
			camera_position[2] = 0;

			camera_rotation[0] = 0;
			camera_rotation[1] = 0;
			camera_rotation[2] = 0;

			isPerspectiveMode = true;
		}
		ImGui::End();
		ModuleUI::updateDraw();

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	void Window::shutdown()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}