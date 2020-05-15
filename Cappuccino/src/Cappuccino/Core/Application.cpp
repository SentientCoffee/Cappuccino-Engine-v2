#include "CappPCH.h"
#include "Cappuccino/Core/Application.h"

#include "Cappuccino/Core/Time.h"

#include "Cappuccino/Rendering/RenderCommand.h"
#include "Cappuccino/Rendering/3D/Renderer.h"
#include "Cappuccino/Rendering/2D/Renderer2D.h"

#include "Cappuccino/Resource/ResourceManager.h"
#include "Cappuccino/Scenes/SceneManager.h"

using namespace Capp;

Application* Application::_instance = nullptr;

Application::Application() :
	Application(100, 100, "Failed to create app!") {}

Application::Application(const unsigned width, const unsigned height, const std::string& title) {
	CAPP_ASSERT(!_instance, "Application already exists!");
	_instance = this;
	const WindowProperties properties = { title, width, height, false, BIND_EVENT_FN(Application::onEvent) };
	_window = Memory::createScope<Window>(properties);

	// Initialize renderers and resource managers
	RenderCommand::init();
	ResourceManager::init();
	SceneManager::init();
}

Application::~Application() {
	RenderCommand::shutdown();
	ResourceManager::shutdown();
	
	_instance = nullptr;
}

void Application::run() {
	if(SceneManager::getCurrentScene() == nullptr) {
		SceneManager::changeScene(0);
	}
	CAPP_ASSERT(SceneManager::getCurrentScene() != nullptr, "No scene has been added to the scene manager!");

	while(_isRunning) {
		Time::preUpdate();
		
		RenderCommand::setClearColour(0.8f, 0.2f, 0.7f, 1.0f);
		RenderCommand::clearScreen();

		if(!_isMinimized) {
			SceneManager::getCurrentScene()->update();
		}
		
		Time::postUpdate();
		_window->update();
	}
}

void Application::onEvent(Event& e) {
	EventDispatcher d(e);
	d.dispatchEventType<WindowClosedEvent>(BIND_EVENT_FN(Application::onWindowClosed));
	d.dispatchEventType<WindowResizedEvent>(BIND_EVENT_FN(Application::onWindowResized));

	SceneManager::getCurrentScene()->onEvent(e);
}


bool Application::onWindowClosed(WindowClosedEvent& e) {
	_isRunning = false;
	return true;
}

bool Application::onWindowResized(WindowResizedEvent& e) {
	if(e.getWidth() == 0 || e.getHeight() == 0) {
		_isMinimized = true;
		return false;
	}

	_isMinimized = false;

	RenderCommand::setViewport(0, 0, e.getWidth(), e.getHeight());
	Renderer::onWindowResized(e.getWidth(), e.getHeight());
	Renderer2D::onWindowResized(e.getWidth(), e.getHeight());
	
	return false;
}

Window* Application::getWindow() const { return _window.get(); }

Application* Application::getInstance() { return _instance; }
