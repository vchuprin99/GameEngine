#include "application.h"

#include <imgui/imgui.h>

#include <iostream>
#include <memory>

class SDK : public GameEngine::Application {
	virtual void onUpdate() override {
		//std::cout << "Update\n";
	}
	virtual void on_UI_draw() override {
        ImGui::Begin("Properties");

        ImGui::SliderFloat3("Scale", scale, -5, 5);
        ImGui::SliderFloat("Rotation", &rotate, 0, 360);
        ImGui::SliderFloat3("Camera Position", camera_position, -5, 5);
        ImGui::SliderFloat3("Camera Rotation", camera_rotation, 0, 360);
        ImGui::Checkbox("Perspective mode", &isPerspectiveMode);
        if (ImGui::Button("Default")) {
            scale[0] = 0.5;
            scale[1] = 0.5;
            scale[2] = 0.5;
            rotate = 0;

            camera_position[0] = 0;
            camera_position[1] = 0;
            camera_position[2] = 0;

            camera_rotation[0] = 0;
            camera_rotation[1] = 0;
            camera_rotation[2] = 0;

            isPerspectiveMode = true;
        }
        ImGui::End();
	}
};

int main() {
    auto sdk = std::make_unique<SDK>();
    sdk->start(1280, 720, "Editor");

    return 0;
}