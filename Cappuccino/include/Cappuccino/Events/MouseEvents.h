#pragma once

#include "Event.h"

namespace Capp {

	class MouseMovedEvent : public Event {
	public:
		
		MouseMovedEvent(float xPos, float yPos);

		EVENT_CLASS_TYPE(MouseMoved);

		inline float getMouseX() const;
		inline float getMouseY() const;

		#if CAPP_DEBUG
		std::string toString() const override;
		#endif

	private:

		float _mouseX, _mouseY;
		
	};

	class MouseScrolledEvent : public Event {
	public:

		MouseScrolledEvent(float xOffset, float yOffset);

		EVENT_CLASS_TYPE(MouseScrolled);

		inline float getXOffset() const;
		inline float getYOffset() const;

		#if CAPP_DEBUG
		std::string toString() const override;
		#endif

	private:

		float _xOffset, _yOffset;

	};

	class MouseButtonPressedEvent : public Event {
	public:

		MouseButtonPressedEvent(int button);

		EVENT_CLASS_TYPE(MouseButtonPressed);

		inline int getMouseButton() const;
		
		#if CAPP_DEBUG
		std::string toString() const override;
		#endif

	private:

		int _mouseButton;
		
	};

	class MouseButtonReleasedEvent : public Event {
	public:

		MouseButtonReleasedEvent(int button);

		EVENT_CLASS_TYPE(MouseButtonReleased);

		inline int getMouseButton() const;
		
		#if CAPP_DEBUG
		std::string toString() const override;
		#endif

	private:

		int _mouseButton;
		
	};
	
}