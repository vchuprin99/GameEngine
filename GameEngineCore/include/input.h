#pragma once

#include "keys.h"

namespace GameEngine {
	class Input {
	public:
		static bool isKeyPressed(const KeyCode keyCode);

		static void pressKey(const KeyCode keyCode);
		static void releaseKey(const KeyCode keyCode);

		static bool isMouseButtonPressed(const MouseButton mouseButtonCode);

		static void pressMouseButton(const MouseButton mouseButtonCode);
		static void releaseMouseButton(const MouseButton mouseButtonCode);
	private:
		static bool s_keysPressed[];
		static bool s_mouseButtonsPressed[];
	};
}