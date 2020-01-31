#pragma once

#include "Cappuccino/Events/Event.h"
#include "Cappuccino/Input/KeyCodes.h"

namespace Capp {

	class KeyPressedEvent : public Event {
	public:
		
		KeyPressedEvent(int keyCode, int repeatCount);

		EVENT_CLASS_TYPE(KeyPressed);
		
		int getKeyCode() const;
		int getRepeatCount() const;

		bool compareKey(KeyCode keyCode) const;

		#if CAPP_DEBUG
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
		bool compareKey(KeyCode keyCode) const;

		#if CAPP_DEBUG
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
		bool compareKey(KeyCode keyCode) const;

		#if CAPP_DEBUG
		std::string toString() const override;
		#endif

	private:

		int _keyCode;

	};
	
}
