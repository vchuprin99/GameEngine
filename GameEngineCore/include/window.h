#pragma once

#include "events.h"

#include <string>
#include <functional>

typedef unsigned int uint;

struct GLFWwindow;

namespace GameEngine {
	using EventCallbackFn = std::function<void(BaseEvent&)>;
	class Window {
	public:
		Window(uint width, uint height, std::string title);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		void on_update();
		inline uint getWidth() const { return winProps.width; }
		inline uint getHeight() const { return winProps.height; }
		inline void setEventCallback(const EventCallbackFn& fn) { winProps.eventCallback = fn; }

	private:
		int init();
		void shutdown();

		struct WinProps {
			uint width, height;
			std::string title;
			EventCallbackFn eventCallback;
		} winProps;
		GLFWwindow* m_window;
	};
}