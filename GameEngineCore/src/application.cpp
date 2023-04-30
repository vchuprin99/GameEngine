#include "application.h"

#include <log.h>

int GameEngine::Application::start(uint width, uint height, const char* title)
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
    m_dispatcher.addEventListener<WindowResizeEvent>([&](WindowResizeEvent& e) {
        //LOG_INFO("Window resize event {0}x{1}", e.getWidth(), e.getHeight());
    });
    m_dispatcher.addEventListener<KeyPressedEvent>([&](KeyPressedEvent& e) {
        //LOG_INFO("Key pressed event {0}", e.getKeyCode());
    });
    m_dispatcher.addEventListener<KeyReleasedEvent>([&](KeyReleasedEvent& e) {
        //LOG_INFO("Key released event {0}", e.getKeyCode());
    });
    m_dispatcher.addEventListener<MouseMovedEvent>([&](MouseMovedEvent& e) {
        //LOG_INFO("Mouse moved event {0}, {1}", e.getX(), e.getY());
    });
    m_dispatcher.addEventListener<MouseScrolledEvent>([&](MouseScrolledEvent& e) {
        //LOG_INFO("Mouse scrolled event {0}", e.getOffset());
    });
    m_dispatcher.addEventListener<MouseButtonPressedEvent>([&](MouseButtonPressedEvent& e) {
        //LOG_INFO("Mouse button pressed event {0}", e.getButton());
    });
    m_dispatcher.addEventListener<MouseButtonReleasedEvent>([&](MouseButtonReleasedEvent& e) {
        //LOG_INFO("Mouse button released event {0}", e.getButton());
    });
   

    while (!m_isWindowClosed) {
        m_window->on_update();
        on_update();
    }

    return 0;
}
