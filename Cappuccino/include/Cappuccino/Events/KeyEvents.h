#pragma once

#include "Event.h"

namespace Capp {

	class KeyPressedEvent : public Event {
	public:
		
		KeyPressedEvent(int keyCode, int repeatCount);

		EVENT_CLASS_TYPE(KeyPressed);
		
		inline int getKeyCode() const;
		inline int getRepeatCount() const;

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

		inline int getKeyCode() const;

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

		inline int getKeyCode() const;

		#if CAPP_DEBUG
		std::string toString() const override;
		#endif

	private:

		int _keyCode;

	};
	
}