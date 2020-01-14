#pragma once

#include "Cappuccino/Core/Window.h"

#include "Cappuccino/Events/Event.h"
#include "Cappuccino/Events/WindowEvents.h"

#include "Cappuccino/Scenes/SceneManager.h"

namespace Capp {

	class Application {
	public:

		Application();
		Application(
			unsigned int       width,
			unsigned int       height,
			const std::string& title
		);
		virtual ~Application();

		void run();
		void onEvent(Event& e);

		const SceneManager& getSceneManager() const;
		Window* getWindow() const;

		static Application* getInstance();

	protected:
		
		SceneManager sceneManager;
		
	private:

		bool onWindowClosed(WindowClosedEvent& e);
		
		Window* _window;

		bool _isRunning = true;
		
		static Application* _instance;
		
	};

	// To be defined in the client app
	Application* createApp();
}