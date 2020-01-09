#include "CappPCH.h"
#include "Cappuccino/Core/Application.h"

#include "Cappuccino/Rendering/RenderCommand.h"

#include <glfw/glfw3.h>

using namespace Capp;

Application* Application::_instance = nullptr;

Application::Application() :
	Application(100, 100, "Failed to create app!") {}

Application::Application(const unsigned width, const unsigned height, const std::string& title) {
	CAPP_ASSERT(!_instance, "Application already exists!");
	_instance = this;

	_window = new Window({ title, width, height, true, BIND_EVENT_FN(Application::onEvent) });
}

Application::~Application() {
	delete _window;
	_instance = nullptr;
}

void Application::run() {
	if(sceneManager.getCurrentScene() == nullptr) {
		sceneManager.changeScene(0);
	}
	CAPP_ASSERT(sceneManager.getCurrentScene() != nullptr, "No scene has been added to the scene manager!");
	
	while(_isRunning) {
		RenderCommand::clearScreen();

		const auto currentTime = static_cast<float>(glfwGetTime());
		const float currentFrameTime = currentTime - _lastFrameTime;
		_lastFrameTime = currentFrameTime;
		
		sceneManager.getCurrentScene()->update(currentFrameTime);
		
		_window->update();
	}
}

void Application::onEvent(Event& e) {
	EventDispatcher d(e);
	d.dispatchEventType<WindowClosedEvent>(BIND_EVENT_FN(Application::onWindowClosed));

	sceneManager.getCurrentScene()->onEvent(e);
}

bool Application::onWindowClosed(WindowClosedEvent& e) {
	_isRunning = false;
	return true;
}

Window* Application::getWindow() const { return _window; }
Application* Application::getInstance() { return _instance; }
