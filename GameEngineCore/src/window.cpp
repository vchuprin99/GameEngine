#include "window.h"

#include "rendering/OpenGL/shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <log.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#define GET_WINPROPS WinProps* winProps = (WinProps*)(glfwGetWindowUserPointer(window))

namespace GameEngine {
    static bool m_gladInitialized = false;

    GLuint vao, points_vbo, color_vbo;
    std::unique_ptr<Shader> shader;

    float points[] = {
        0, 0.5, 0,
        0.5, -0.5, 0,
        -0.5, -0.5, 0
    };
    float color[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };
    const char* vertexShader = R"(
        #version 460

        layout (location = 0) in vec3 pos;
        layout (location = 1) in vec3 color;

        out vec4 vertexColor;

        void main(){
            gl_Position = vec4(pos, 1);
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

        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &color_vbo);

        glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        shader = std::make_unique<Shader>(vertexShader, fragmentShader);

        return 0;
    }

    void Window::on_update()
    {
        shader->bind();
        glClearColor(0.5, 0.5, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_DYNAMIC_DRAW);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(getWidth());
        io.DisplaySize.y = static_cast<float>(getHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Properties");

        ImGui::ColorEdit3("Color 1", color);
        ImGui::ColorEdit3("Color 2", color + 3);
        ImGui::ColorEdit3("Color 3", color + 6);

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