#include "application.h"

#include "rendering/OpenGL/shader.h"
#include "rendering/OpenGL/vertexBuffer.h"
#include "rendering/OpenGL/vertexArray.h"
#include "rendering/OpenGL/indexBuffer.h"
#include "rendering/camera.h"
#include "rendering/OpenGL/openGL_Renderer.h"
#include "modules/moduleUI.h"

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

    int Application::start(uint width, uint height, const char* title)
    {
        LOG_INFO("Application started");

        m_window = std::make_unique<Window>(width, height, title);
        m_window->setEventCallback([&](BaseEvent& e) {
            m_dispatcher.dispacth(e);
            });

        m_dispatcher.addEventListener<WindowCloseEvent>([&](WindowCloseEvent& e) {
            //LOG_INFO("Window close event");
            m_isWindowClosed = true;

            });

        // ==========================================================================================
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
        // =========================================================================================

        while (!m_isWindowClosed) {
            shader->bind();
            indexBuffer->bind();
            shader->setFloat("aspect_ratio", m_window->getAspect());
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