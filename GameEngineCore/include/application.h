#pragma once

#include "window.h"

#include "rendering/camera.h"

#include <memory>

namespace GameEngine {
	class Application {
	public:
		Application() {}
		virtual ~Application() {}

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		virtual int start(uint width, uint height, const char* title);
		virtual void onUpdate() {}
		virtual void on_UI_draw() {}
		
		float scale[3] = { 0.5, 0.5, 0.5 };
		float rotate = 0;
		float camera_position[3] = { 0 };
		float camera_rotation[3] = { 0 };
		bool isPerspectiveMode = true;
		Camera camera;
	private:
		std::unique_ptr<Window> m_window;
		EventDispathcer m_dispatcher;

		bool m_isWindowClosed = false;
	};
}