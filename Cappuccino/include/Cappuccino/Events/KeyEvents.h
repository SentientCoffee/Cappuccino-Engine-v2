#pragma once

#include "Cappuccino/Events/Event.h"

namespace Capp {

	class KeyPressedEvent : public Event {
	public:
		
		KeyPressedEvent(const int keyCode, const int repeatCount) : _keyCode(keyCode), _repeatCount(repeatCount) {}

		EVENT_CLASS_TYPE(KeyPressed);
		
		int getKeyCode() const { return _keyCode; }
		int getRepeatCount() const { return _repeatCount; }

#if CAPP_DEBUG || CAPP_RELEASE

		std::string toString() const override {
			return std::string("KeyPressed: " + std::to_string(_keyCode) + " (" + std::to_string(_repeatCount) + " repeat(s))");
		}
		
#endif
		
	private:

		int _keyCode, _repeatCount;
		
	};

	class KeyReleasedEvent: public Event {
	public:

		KeyReleasedEvent(const int keyCode) : _keyCode(keyCode) {}

		EVENT_CLASS_TYPE(KeyReleased);

		int getKeyCode() const { return _keyCode; }

#if CAPP_DEBUG || CAPP_RELEASE
		
		std::string toString() const override {
			return std::string("KeyReleased: " + std::to_string(_keyCode));
		}
		
#endif

	private:

		int _keyCode;

	};

	class KeyTypedEvent : public Event {
	public:

		KeyTypedEvent(const int keyCode) : _keyCode(keyCode) {}

		EVENT_CLASS_TYPE(KeyReleased);

		int getKeyCode() const { return _keyCode; }

#if CAPP_DEBUG || CAPP_RELEASE
		
		std::string toString() const override {
			return std::string("KeyTyped: " + std::to_string(_keyCode));
		}
		
#endif

	private:

		int _keyCode;

	};
	
}
