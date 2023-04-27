#include "window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <log.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#define GET_WINPROPS WinProps* winProps = (WinProps*)(glfwGetWindowUserPointer(window))

namespace GameEngine {
    bool m_gladInitialized = false;
}

GameEngine::Window::Window(uint width, uint height, std::string title)
	: winProps({width, height, std::move(title)})
{
	LOG_INFO("Window created {0}x{1}", width, height);
	int res = init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
}

GameEngine::Window::~Window()
{
	shutdown();
}

int GameEngine::Window::init()
{
    if (!glfwInit()) {
        return -1;
    }

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
    }

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        GET_WINPROPS;
        winProps->width = width;
        winProps->height = height;

        WindowResizeEvent event(width, height);
        winProps->eventCallback(event);
    });
    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        GET_WINPROPS;

        WindowCloseEvent event;
        winProps->eventCallback(event);
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

	return 0;
}

void GameEngine::Window::on_update()
{
    glClear(GL_COLOR_BUFFER_BIT);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = static_cast<float>(getWidth());
    io.DisplaySize.y = static_cast<float>(getHeight());

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ImGui::EndFrame();

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void GameEngine::Window::shutdown()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();

}