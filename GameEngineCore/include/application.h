#pragma once

#include "window.h"

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
	private:
		virtual void on_update() {}
		
		std::unique_ptr<Window> m_window;
		EventDispathcer m_dispatcher;

		bool m_isWindowClosed = false;
	};
}