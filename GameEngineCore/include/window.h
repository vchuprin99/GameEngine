#pragma once

#include "events.h"

#include <string>
#include <functional>

#include <glm/vec2.hpp>

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

		void onUpdate();
		inline uint getWidth() const { return winProps.width; }
		inline uint getHeight() const { return winProps.height; }
		inline float getAspect() const { return winProps.aspect_ratio; }
		inline void setEventCallback(const EventCallbackFn& fn) { winProps.eventCallback = fn; }
		glm::vec2 getCursorPos() const;
		void enableCursor();
		void disableCursor();
		void setCursorPos(const glm::vec2& pos);

	private:
		int init();
		void shutdown();

		struct WinProps {
			uint width, height;
			float aspect_ratio;
			std::string title;
			EventCallbackFn eventCallback;
		} winProps;
		GLFWwindow* m_window;
	};
}