#include "application.h"

#include <iostream>
#include <memory>

class SDK : public GameEngine::Application {
	virtual void on_update() override {
		//std::cout << "Update\n";
	}
};

int main() {
	auto app = std::make_unique<SDK>();
	app->start(1280, 720, "My window");

	return 0;
}