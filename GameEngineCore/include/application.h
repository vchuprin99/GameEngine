#pragma once

#include "window.h"

#include "camera.h"

#include <memory>

namespace GameEngine {
	class Application {
	public:
		Application();
		virtual ~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		virtual int start(uint width, uint height, const char* title);
		virtual void onUpdate() {}
		virtual void on_UI_draw() {}

		virtual void onKeyPressed(const KeyCode key) {}
		virtual void onMouseButtonPressed(const MouseButton mouseButton) {}
		virtual void onMouseMoved(double x, double y) {}

		inline glm::vec2 getCursorPos() const { return m_window->getCursorPos(); }
		inline void setCursorPos(const glm::vec2& pos) { m_window->setCursorPos(pos); }
		inline const bool isCursorEnabled() const { return m_isCursorEnabled; }
		inline void enableCursor() { m_isCursorEnabled = true; m_window->enableCursor(); }
		inline void disableCursor() { m_isCursorEnabled = false; m_window->disableCursor(); }
		
		bool isPerspectiveMode = true;
		float camera_speed = 0.05;
		float sensivity = 1;
		Camera camera;

		glm::vec2 lastCursorPos;
	private:
		std::unique_ptr<Window> m_window;
		EventDispathcer m_dispatcher;

		bool m_isWindowClosed = false;
		bool m_isCursorEnabled = true;
	};
}