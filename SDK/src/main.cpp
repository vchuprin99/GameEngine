#include "application.h"

#include "input.h"

#include <imgui/imgui.h>
#include <log.h>
#include <iostream>
#include <memory>


class SDK : public GameEngine::Application {
	void setupDockspaceMenu()
	{
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
		static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar(3);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		ImGui::End();
	}

	virtual void onUpdate() override {

		glm::vec3 move_delta = { 0, 0, 0 };
		glm::vec3 rotate_delta = { 0, 0, 0 };

		if (GameEngine::Input::isKeyPressed(GameEngine::KeyCode::KEY_W)) {
			move_delta.x += camera_speed;
		}
		else if (GameEngine::Input::isKeyPressed(GameEngine::KeyCode::KEY_S)) {
			move_delta.x -= camera_speed;
		}
		if (GameEngine::Input::isKeyPressed(GameEngine::KeyCode::KEY_D)) {
			move_delta.y += camera_speed;
		}
		else if (GameEngine::Input::isKeyPressed(GameEngine::KeyCode::KEY_A)) {
			move_delta.y -= camera_speed;
		}
		if (GameEngine::Input::isKeyPressed(GameEngine::KeyCode::KEY_SPACE)) {
			move_delta.z += camera_speed;
		}
		else if (GameEngine::Input::isKeyPressed(GameEngine::KeyCode::KEY_LEFT_SHIFT)) {
			move_delta.z -= camera_speed;
		}

		if (GameEngine::Input::isKeyPressed(GameEngine::KeyCode::KEY_UP)) {
			rotate_delta.y -= 0.01;
		}
		else if (GameEngine::Input::isKeyPressed(GameEngine::KeyCode::KEY_DOWN)) {
			rotate_delta.y += 0.01;
		}
		if (GameEngine::Input::isKeyPressed(GameEngine::KeyCode::KEY_RIGHT)) {
			rotate_delta.z += 0.01;
		}
		else if (GameEngine::Input::isKeyPressed(GameEngine::KeyCode::KEY_LEFT)) {
			rotate_delta.z -= 0.01;
		}
		if (GameEngine::Input::isKeyPressed(GameEngine::KeyCode::KEY_P)) {
			rotate_delta.x += 0.01;
		}
		else if (GameEngine::Input::isKeyPressed(GameEngine::KeyCode::KEY_Q)) {
			rotate_delta.x -= 0.01;
		}
		camera.moveAndRotate(move_delta, rotate_delta);

	}

	virtual void onKeyPressed(const GameEngine::KeyCode key) override {
		if (key == GameEngine::KeyCode::KEY_ESCAPE) {
			if (isCursorEnabled())
			{
				lastCursorPos = getCursorPos();
				disableCursor();
			}
			else
			{
				enableCursor();
			}
		}
	}
	virtual void onMouseMoved(double x, double y) override {
		if (!isCursorEnabled()) {
			double xOffset = x - lastCursorPos.x;
			double yOffset = y - lastCursorPos.y;
			lastCursorPos.x = x;
			lastCursorPos.y = y;

			camera.rotate({ 0, yOffset * 0.001 * sensivity, xOffset * 0.001 * sensivity });
		}
	}

	virtual void on_UI_draw() override {
		setupDockspaceMenu();

		ImGui::Begin("Settings");

		ImGui::SliderFloat("Camera speed", &camera_speed, 0, 0.5);
		ImGui::SliderFloat("Sensivity", &sensivity, 0, 3);
		ImGui::Checkbox("Perspective mode", &isPerspectiveMode);
		if (ImGui::Button("Default positions")) {
			camera.setPositionRotation({ 0, 0, 0 }, { 0, 0, 0 });
		}
		ImGui::End();
	}
};

int main() {
	auto sdk = std::make_unique<SDK>();
	sdk->start(1280, 720, "Editor");

	return 0;
}