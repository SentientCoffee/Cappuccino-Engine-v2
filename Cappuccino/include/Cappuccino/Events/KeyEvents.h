#pragma once

#include "Cappuccino/Events/Event.h"

namespace Capp {

	class KeyPressedEvent : public Event {
	public:
		
		KeyPressedEvent(int keyCode, int repeatCount);

		EVENT_CLASS_TYPE(KeyPressed);
		
		int getKeyCode() const;
		int getRepeatCount() const;

		#if CAPP_DEBUG || CAPP_RELEASE
		std::string toString() const override;
		#endif
		
	private:

		int _keyCode, _repeatCount;
		
	};

	class KeyReleasedEvent: public Event {
	public:

		KeyReleasedEvent(int keyCode);

		EVENT_CLASS_TYPE(KeyReleased);

		int getKeyCode() const;

		#if CAPP_DEBUG || CAPP_RELEASE
		std::string toString() const override;
		#endif

	private:

		int _keyCode;

	};

	class KeyTypedEvent : public Event {
	public:

		KeyTypedEvent(int keyCode);

		EVENT_CLASS_TYPE(KeyReleased);

		int getKeyCode() const;

		#if CAPP_DEBUG || CAPP_RELEASE
		std::string toString() const override;
		#endif

	private:

		int _keyCode;

	};
	
}
