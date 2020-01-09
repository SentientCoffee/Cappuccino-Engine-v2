#include "CappPCH.h"
#include "Cappuccino/Events/WindowEvents.h"

using namespace Capp;

WindowResizedEvent::WindowResizedEvent(const unsigned int width, const unsigned int height) :
	_width(width), _height(height) {}
	
unsigned WindowResizedEvent::getWidth() const { return _width; }
unsigned WindowResizedEvent::getHeight() const { return _height; }

#if CAPP_DEBUG
std::string WindowResizedEvent::toString() const {
	return std::string("WindowResized: " + std::to_string(_width) + " x " + std::to_string(_height));
}
#endif