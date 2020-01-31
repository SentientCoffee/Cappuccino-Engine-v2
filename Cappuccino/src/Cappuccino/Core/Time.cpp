#include "CappPCH.h"
#include "Cappuccino/Core/Time.h"

#include <glfw/glfw3.h>

using namespace Capp;

float Time::_lastFrameTime = 0.0f;
float Time::_deltaTime = 0.0f;
float Time::_totalAppTime = 0.0f;

float Time::getTime() { return _totalAppTime; }
float Time::getDeltaTime() { return _deltaTime > 0.0f ? _deltaTime : 1.0f / 60.0f; }

void Time::preUpdate() {
	_totalAppTime = static_cast<float>(glfwGetTime());
	_deltaTime = _totalAppTime - _lastFrameTime;
}

void Time::postUpdate() {
	_lastFrameTime = _totalAppTime;
}
