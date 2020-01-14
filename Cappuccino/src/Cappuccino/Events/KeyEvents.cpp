#include "CappPCH.h"
#include "Cappuccino/Events/KeyEvents.h"

using namespace Capp;

KeyPressedEvent::KeyPressedEvent(const int keyCode, const int repeatCount) :
	_keyCode(keyCode), _repeatCount(repeatCount) {}

int KeyPressedEvent::getKeyCode() const { return _keyCode; }
int KeyPressedEvent::getRepeatCount() const { return _repeatCount; }

KeyReleasedEvent::KeyReleasedEvent(const int keyCode) :
	_keyCode(keyCode) {}

int KeyReleasedEvent::getKeyCode() const { return _keyCode; }

KeyTypedEvent::KeyTypedEvent(const int keyCode) :
	_keyCode(keyCode) {}

int KeyTypedEvent::getKeyCode() const { return _keyCode; }

#if CAPP_DEBUG

std::string KeyPressedEvent::toString() const {
	return std::string("KeyPressed: " + std::to_string(_keyCode) + " (" + std::to_string(_repeatCount) + " repeat(s))");
}

std::string KeyReleasedEvent::toString() const {
	return std::string("KeyReleased: " + std::to_string(_keyCode));
}

std::string KeyTypedEvent::toString() const {
	return std::string("KeyTyped: " + std::to_string(_keyCode));
}

#endif