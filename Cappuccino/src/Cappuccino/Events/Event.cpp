#include "CappPCH.h"
#include "Cappuccino/Events/Event.h"

using namespace Capp;

bool Event::isHandled() const { return _isHandled; }

#if CAPP_DEBUG || CAPP_RELEASE
std::string Event::toString() const { return getName(); }
#endif

EventDispatcher::EventDispatcher(Event& e) : _event(e) {}
