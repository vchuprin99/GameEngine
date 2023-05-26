#include "window.h"

#include "rendering/OpenGL/shader.h"
#include "rendering/OpenGL/vertexBuffer.h"
#include "rendering/OpenGL/vertexArray.h"
#include "rendering/OpenGL/indexBuffer.h"
#include "rendering/camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <log.h>

#include <glm/mat4x4.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#define GET_WINPROPS WinProps* winProps = reinterpret_cast<WinProps*>(glfwGetWindowUserPointer(window))

namespace GameEngine {
	static bool m_gladInitialized = false;

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

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	}

	Window::~Window()
	{
		shutdown();
	}

	int Window::init()
	{
		if (!glfwInit()) {
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_window = glfwCreateWindow(winProps.width, winProps.height, winProps.title.c_str(), NULL, NULL);
		if (!m_window)
		{
			LOG_ERR("Unable to create window");
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, &winProps);

		if (!m_gladInitialized) {
			if (!gladLoadGL()) {
				LOG_ERR("Unable to init glad");
				return -1;
			}
			LOG_INFO("GLAD initialized");
			m_gladInitialized = true;
		}

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
			GET_WINPROPS;
			winProps->width = width;
			winProps->height = height;
			winProps->aspect_ratio = (float)width / (float)height;

			WindowResizeEvent event(width, height);
			winProps->eventCallback(event);
			});
		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
			GET_WINPROPS;

			WindowCloseEvent event;
			winProps->eventCallback(event);
			});
		glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
			});
		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			GET_WINPROPS;

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
			GET_WINPROPS;

			MouseMovedEvent event(xpos, ypos);
			winProps->eventCallback(event);
			});
		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset) {
			GET_WINPROPS;

			MouseScrolledEvent event(xoffset);
			winProps->eventCallback(event);
			});
		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
			GET_WINPROPS;

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
		shader->setFloat("aspect_ratio", getAspect());
		shader->setMat4("model_matrix", model_matrix);

		glClearColor(0.5, 0.5, 0.5, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		indexBuffer->bind();
		glDrawElements(GL_TRIANGLES, vertexArray->getIndicesCount(), GL_UNSIGNED_INT, nullptr);

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(getWidth());
		io.DisplaySize.y = static_cast<float>(getHeight());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

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

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	void Window::shutdown()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}