#pragma once

#include <functional>
#include <array>

#define EVENT_CLASS_TYPE(x) virtual EventType getType() const override { return EventType::x; }\
							static EventType getTypeStatic() { return EventType::x; }

namespace GameEngine {
	enum class EventType {
		WindowClose = 0,
		WindowResize,
		
		KeyPressed,
		KeyReleased,

		MouseMoved,
		MouseScrolled,
		MouseButtonPressed,
		MouseButtonReleased,


		EventsCount
	};

	class BaseEvent {
	public:
		virtual EventType getType() const = 0;
	};

	class EventDispathcer {
	public:
		template<typename Type>
		void addEventListener(std::function<void(Type&)> callback) {
			auto baseCallback = [func = std::move(callback)](BaseEvent& e) {
				if (e.getType() == Type::getTypeStatic()) {
					func(static_cast<Type&>(e));	
				}
			};

			m_eventCallbacks[static_cast<size_t>(Type::getTypeStatic())] = std::move(baseCallback);
		}
		void dispacth(BaseEvent& event) {
			auto& callback = m_eventCallbacks[static_cast<size_t>(event.getType())];

			if (callback) {
				callback(event);
			}
		}
	private:
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> m_eventCallbacks;
	};

	class WindowCloseEvent : public BaseEvent {
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WindowClose);
	};
	class WindowResizeEvent : public BaseEvent {
	public:
		WindowResizeEvent(int width, int height)
			: m_width(width), m_height(height) {}

		EVENT_CLASS_TYPE(WindowResize);

		inline int getWidth() const { return m_width; }
		inline int getHeight() const { return m_height; }
	private:
		int m_width, m_height;
	};
	class KeyPressedEvent : public BaseEvent {
	public:
		KeyPressedEvent(int keyCode, const bool repeated)
		: m_keyCode(keyCode), m_repeated(repeated) {}

		EVENT_CLASS_TYPE(KeyPressed);

		inline int getKeyCode() const { return m_keyCode; }
		inline bool isRepeated() const { return m_repeated; }
	private:
		int m_keyCode;
		bool m_repeated;
	};
	class KeyReleasedEvent : public BaseEvent {
	public:
		KeyReleasedEvent(int keyCode)
			: m_keyCode(keyCode) {}

		EVENT_CLASS_TYPE(KeyReleased);

		inline int getKeyCode() const { return m_keyCode; }
	private:
		int m_keyCode;
	};
	class MouseMovedEvent : public BaseEvent {
	public:
		MouseMovedEvent(double x, double y)
			: m_x(x), m_y(y) {}

		EVENT_CLASS_TYPE(MouseMoved);

		inline double getX() const { return m_x; }
		inline double getY() const { return m_y; }
	private:
		double m_x, m_y;
	};
	class MouseScrolledEvent : public BaseEvent {
	public:
		MouseScrolledEvent(double offset)
			: m_offset(offset) {}

		EVENT_CLASS_TYPE(MouseScrolled);

		inline double getOffset() const { return m_offset; }
	private:
		double m_offset;
	};
	class MouseButtonPressedEvent : public BaseEvent {
	public:
		MouseButtonPressedEvent(int button)
			: m_button(button) {}

		EVENT_CLASS_TYPE(MouseButtonPressed);

		inline int getButton() const { return m_button; }
	private:
		int m_button;
	};
	class MouseButtonReleasedEvent : public BaseEvent {
	public:
		MouseButtonReleasedEvent(int button)
			: m_button(button) {}

		EVENT_CLASS_TYPE(MouseButtonReleased);

		inline int getButton() const { return m_button; }
	private:
		int m_button;
	};
}