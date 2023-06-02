#include "application.h"

#include "rendering/OpenGL/shader.h"
#include "rendering/OpenGL/vertexBuffer.h"
#include "rendering/OpenGL/vertexArray.h"
#include "rendering/OpenGL/indexBuffer.h"
#include "camera.h"
#include "rendering/OpenGL/openGL_Renderer.h"
#include "modules/moduleUI.h"
#include "input.h"

#include <imgui/imgui.h>

#include <glm/mat4x4.hpp>

#include <GLFW/glfw3.h>
#include <log.h>
#include <memory>

namespace GameEngine {
    std::unique_ptr<Shader> shader;
    std::unique_ptr<VertexArray> vertexArray;
    std::unique_ptr<VertexBuffer> vertexBuffer;
    std::unique_ptr<IndexBuffer> indexBuffer;

    glm::mat4 scale_matrix;
    glm::mat4 rotate_matrix;
    glm::mat4 model_matrix;

    float rotate;

    float points[] = {
        -0.5,  0.5,  0.5,      1, 1, 1, // 0
        -0.5,  0.5, -0.5,      1, 1, 1, // 1
        -0.5, -0.5,  0.5,      1, 1, 1, // 2
        -0.5, -0.5, -0.5,      1, 1, 1, // 3
           
        0.5,  0.5,  0.5,      1, 0, 0, // 4
        0.5,  0.5, -0.5,      0, 1, 0, // 5
        0.5, -0.5,  0.5,      0, 0, 1, // 6
        0.5, -0.5, -0.5,      0, 1, 1  // 7
    };
    GLuint indices[] = { 
        0, 1, 2, 1, 2, 3, 
        4, 5, 6, 5, 6, 7,
    };
    
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

    Application::Application()
        : camera({ 0, 0, 2 })
    {

    }
    Application::~Application()
    {

    }

    int Application::start(uint width, uint height, const char* title)
    {
        LOG_INFO("Application started");

        m_window = std::make_unique<Window>(width, height, title);
        lastCursorPos = getCursorPos();

        m_window->setEventCallback([&](BaseEvent& e) {
            m_dispatcher.dispacth(e);
        });

        m_dispatcher.addEventListener<WindowCloseEvent>([&](WindowCloseEvent& e) {
            //LOG_INFO("Window close event");
            m_isWindowClosed = true;

        });
        m_dispatcher.addEventListener<KeyPressedEvent>([&](KeyPressedEvent& e) {
            Input::pressKey(e.getKeyCode());
            onKeyPressed(e.getKeyCode());
        });
        m_dispatcher.addEventListener<KeyReleasedEvent>([&](KeyReleasedEvent& e) {
            Input::releaseKey(e.getKeyCode());
        });
        m_dispatcher.addEventListener<MouseButtonPressedEvent>([&](MouseButtonPressedEvent& e) {
            Input::pressMouseButton(e.getButton());
            onMouseButtonPressed(e.getButton());
        });
        m_dispatcher.addEventListener<MouseButtonReleasedEvent>([&](MouseButtonReleasedEvent& e) {
            Input::releaseMouseButton(e.getButton());
        });
        m_dispatcher.addEventListener<MouseMovedEvent>([&](MouseMovedEvent& e) {
            //LOG_INFO("Mouse moved on {0}x{1}", e.getX(), e.getY());
            onMouseMoved(e.getX(), e.getY());
        });

        // ==========================================================================================
        BufferLayout bufferLayout {
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };
        vertexArray = std::make_unique<VertexArray>();
        vertexBuffer = std::make_unique<VertexBuffer>(points, sizeof(points), bufferLayout);

        indexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

        vertexArray->addVertexBuffer(*vertexBuffer);
        vertexArray->setIndexBuffer(*indexBuffer);

        shader = std::make_unique<Shader>(vertexShader, fragmentShader);
        // =========================================================================================

        while (!m_isWindowClosed) {
            shader->bind();
            indexBuffer->bind();
            shader->setFloat("aspect_ratio", m_window->getAspect());
            shader->setMat4("model_matrix", model_matrix);

            OpenGL_Renderer::clear();

            scale_matrix = {
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
            
            rotate += 0.01;
            rotate_matrix = {
                cos(rotate), -sin(rotate), 0, 0,
                sin(rotate), cos(rotate), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            };
            model_matrix = rotate_matrix * scale_matrix;

            camera.setProjectionMode(
                isPerspectiveMode ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic
            );
            shader->setMat4("view_matrix", camera.getViewMatrix());
            shader->setMat4("projection_matrix", camera.getProjectionMatrix());

            // =========================================================================================
            OpenGL_Renderer::draw(*vertexArray);
            ModuleUI::updateBegin();
            on_UI_draw();
            ModuleUI::updateDraw();
            // =========================================================================================

            m_window->onUpdate();
            onUpdate();
        }

        return 0;
    }
}