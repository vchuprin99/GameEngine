#include "keys.h"
#include "input.h"

bool GameEngine::Input::s_keysPressed[static_cast<size_t>(KeyCode::KEY_LAST)] = {};
bool GameEngine::Input::s_mouseButtonsPressed[static_cast<size_t>(MouseButton::KEY_LAST) + 1] = {};

bool GameEngine::Input::isKeyPressed(const KeyCode keyCode)
{
	return s_keysPressed[static_cast<size_t>(keyCode)];
}

void GameEngine::Input::pressKey(const KeyCode keyCode)
{
	s_keysPressed[static_cast<size_t>(keyCode)] = true;
}

void GameEngine::Input::releaseKey(const KeyCode keyCode)
{
	s_keysPressed[static_cast<size_t>(keyCode)] = false;
}

bool GameEngine::Input::isMouseButtonPressed(const MouseButton mouseButtonCode)
{
	return s_mouseButtonsPressed[static_cast<size_t>(mouseButtonCode)];
}

void GameEngine::Input::pressMouseButton(const MouseButton mouseButtonCode)
{
	s_mouseButtonsPressed[static_cast<size_t>(mouseButtonCode)] = true;
}

void GameEngine::Input::releaseMouseButton(const MouseButton mouseButtonCode)
{
	s_mouseButtonsPressed[static_cast<size_t>(mouseButtonCode)] = false;
}
