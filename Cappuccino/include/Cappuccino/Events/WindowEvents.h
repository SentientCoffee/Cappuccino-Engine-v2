#pragma once

#include "Event.h"

namespace Capp {
	
	class WindowResizedEvent : public Event {
	public:

		WindowResizedEvent(const uint32_t width, const uint32_t height) :
			_width(width), _height(height) {}

		EVENT_CLASS_TYPE(WindowResized);

		uint32_t getWidth() const { return _width; }
		uint32_t getHeight() const { return _height; }

#if CAPP_DEBUG || CAPP_RELEASE
		std::string toString() const override {
			return std::string("WindowResized: " + std::to_string(_width) + " x " + std::to_string(_height));
		}
#endif

	private:

		uint32_t _width, _height;
		
	};
	
	class WindowClosedEvent : public Event {
	public:

		WindowClosedEvent() = default;

		EVENT_CLASS_TYPE(WindowClosed);
		
	};
}