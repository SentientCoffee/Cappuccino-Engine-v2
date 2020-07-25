#pragma once

#include "Event.h"

namespace Capp {

	class MouseMovedEvent : public Event {
	public:
		
		MouseMovedEvent(const float xPos, const float yPos) : _mouseX(xPos), _mouseY(yPos) {}

		EVENT_CLASS_TYPE(MouseMoved);

		float getMouseX() const { return _mouseX; }
		float getMouseY() const { return _mouseY; }

#if CAPP_DEBUG || CAPP_RELEASE
		
		std::string toString() const override {
			return std::string("MouseMoved: " + std::to_string(static_cast<int>(_mouseX)) + ", " + std::to_string(static_cast<int>(_mouseY)));
		}
		
#endif

	private:

		float _mouseX, _mouseY;
		
	};

	class MouseScrolledEvent : public Event {
	public:

		MouseScrolledEvent(const float xOffset, const float yOffset) : _xOffset(xOffset), _yOffset(yOffset) {}

		EVENT_CLASS_TYPE(MouseScrolled);

		float getXOffset() const { return _xOffset; }
		float getYOffset() const { return _yOffset; }

#if CAPP_DEBUG || CAPP_RELEASE
		
		std::string toString() const override {
			return std::string("MouseScrolled: " + std::to_string(static_cast<int>(_xOffset)) + ", " + std::to_string(static_cast<int>(_yOffset)));
		}
		
#endif

	private:

		float _xOffset, _yOffset;

	};

	class MouseButtonPressedEvent : public Event {
	public:

		MouseButtonPressedEvent(const int button) : _mouseButton(button) {}

		EVENT_CLASS_TYPE(MouseButtonPressed);

		int getMouseButton() const { return _mouseButton; }
		
#if CAPP_DEBUG || CAPP_RELEASE
		
		std::string toString() const override {
			return std::string("MouseButtonPressed: " + std::to_string(_mouseButton));
		}
		
#endif

	private:

		int _mouseButton;
		
	};

	class MouseButtonReleasedEvent : public Event {
	public:

		MouseButtonReleasedEvent(const int button) : _mouseButton(button) {}

		EVENT_CLASS_TYPE(MouseButtonReleased);

		int getMouseButton() const { return _mouseButton; }
		
#if CAPP_DEBUG || CAPP_RELEASE
		
		std::string toString() const override {
			return std::string("MouseButtonReleased: " + std::to_string(_mouseButton));
		}
		
#endif

	private:

		int _mouseButton;
		
	};
	
}
