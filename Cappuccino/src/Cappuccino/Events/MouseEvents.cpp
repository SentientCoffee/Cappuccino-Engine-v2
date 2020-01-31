#include "CappPCH.h"
#include "Cappuccino/Events/MouseEvents.h"

using namespace Capp;

MouseMovedEvent::MouseMovedEvent(const float xPos, const float yPos) :
	_mouseX(xPos), _mouseY(yPos) {}

float MouseMovedEvent::getMouseX() const { return _mouseX; }
float MouseMovedEvent::getMouseY() const { return _mouseY; }

MouseScrolledEvent::MouseScrolledEvent(const float xOffset, const float yOffset) :
	_xOffset(xOffset), _yOffset(yOffset) {}

float MouseScrolledEvent::getXOffset() const { return _xOffset; }
float MouseScrolledEvent::getYOffset() const { return _yOffset; }

MouseButtonPressedEvent::MouseButtonPressedEvent(const int button) :
	_mouseButton(button) {}

int MouseButtonPressedEvent::getMouseButton() const { return _mouseButton; }
bool MouseButtonPressedEvent::compareMouseButton(const MouseButton mouseButton) const { return _mouseButton == static_cast<int>(mouseButton); }

MouseButtonReleasedEvent::MouseButtonReleasedEvent(const int button) :
	_mouseButton(button) {}

int MouseButtonReleasedEvent::getMouseButton() const { return _mouseButton; }
bool MouseButtonReleasedEvent::compareMouseButton(const MouseButton mouseButton) const { return _mouseButton == static_cast<int>(mouseButton); }

#if CAPP_DEBUG

std::string MouseMovedEvent::toString() const {
	return std::string("MouseMoved: " + std::to_string(static_cast<int>(_mouseX)) + ", " + std::to_string(static_cast<int>(_mouseY)));
}

std::string MouseScrolledEvent::toString() const {
	return std::string("MouseScrolled: " + std::to_string(static_cast<int>(_xOffset)) + ", " + std::to_string(static_cast<int>(_yOffset)));
}

std::string MouseButtonPressedEvent::toString() const {
	return std::string("MouseButtonPressed: " + std::to_string(_mouseButton));
}
std::string MouseButtonReleasedEvent::toString() const {
	return std::string("MouseButtonReleased: " + std::to_string(_mouseButton));
}

#endif
