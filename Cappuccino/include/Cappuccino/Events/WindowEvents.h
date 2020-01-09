#pragma once

#include "Event.h"

namespace Capp {
	
	class WindowResizedEvent : public Event {
	public:

		WindowResizedEvent(unsigned int width, unsigned int height);

		EVENT_CLASS_TYPE(WindowResized);

		inline unsigned getWidth() const;
		inline unsigned getHeight() const;

		#if CAPP_DEBUG
		std::string toString() const override;
		#endif

	private:

		unsigned _width, _height;
		
	};
	
	class WindowClosedEvent : public Event {
	public:

		WindowClosedEvent() = default;

		EVENT_CLASS_TYPE(WindowClosed);
		
	};
}