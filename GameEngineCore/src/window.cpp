#include "window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <log.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#define GET_WINPROPS WinProps* winProps = (WinProps*)(glfwGetWindowUserPointer(window))

namespace GameEngine {
    static bool m_gladInitialized = false;

    GLuint vao, vbo;
    float points[] = {
        0, 0.5, 0,
        0.5, -0.5, 0,
        -0.5, -0.5, 0
    };
    float color[3] = {
        0, 0, 0
    };

    Window::Window(uint width, uint height, std::string title)
        : winProps({ width, height, std::move(title) })
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

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        return 0;
    }

    void Window::on_update()
    {
        glClearColor(0.5, 0.5, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(getWidth());
        io.DisplaySize.y = static_cast<float>(getHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Properties");

        ImGui::ColorEdit3("Color", color);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    void Window::shutdown()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
}