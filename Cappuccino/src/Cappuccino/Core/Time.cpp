#include "CappPCH.h"
#include "Cappuccino/Core/Time.h"

#include <glfw/glfw3.h>

using namespace Capp;

float Time::_lastFrameTime = 0.0f;
float Time::_currentFrameTime = 0.0f;

float Time::getTime() { return static_cast<float>(glfwGetTime()); }

float Time::getDeltaTime() { return _currentFrameTime > 0.0f ? _currentFrameTime : 1.0f / 60.0f; }

void Time::calculateDeltaTime() {
	_currentFrameTime = getTime() - _lastFrameTime;
	_lastFrameTime = _currentFrameTime;
}
