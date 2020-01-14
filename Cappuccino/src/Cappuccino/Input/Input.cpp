#include "CappPCH.h"
#include "Cappuccino/Input/Input.h"

#include "Cappuccino/Core/Application.h"

#include <glfw/glfw3.h>

using namespace Capp;

bool Input::keyPressed(KeyCode key) {
	const int state = glfwGetKey(Application::getInstance()->getWindow()->getGlfwWindow(), static_cast<int>(key));
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::keyReleased(KeyCode key) {
	return glfwGetKey(Application::getInstance()->getWindow()->getGlfwWindow(), static_cast<int>(key)) == GLFW_RELEASE;
}

bool Input::mouseButtonPressed(MouseButton button) {
	return glfwGetMouseButton(Application::getInstance()->getWindow()->getGlfwWindow(), static_cast<int>(button)) == GLFW_PRESS;
}

bool Input::mouseButtonReleased(MouseButton button) {
	return glfwGetKey(Application::getInstance()->getWindow()->getGlfwWindow(), static_cast<int>(button)) == GLFW_RELEASE;
}

glm::vec2 Input::getMousePosition() {
	double xPos, yPos;
	glfwGetCursorPos(Application::getInstance()->getWindow()->getGlfwWindow(), &xPos, &yPos);
	return { static_cast<float>(xPos), static_cast<float>(yPos) };
}