#pragma once

struct GLFWwindow;

namespace GameEngine {
	class ModuleUI {
	public:
		static void init(GLFWwindow* pWindow);
		static void shutdown();

		static void updateBegin();
		static void updateDraw();
	};
}