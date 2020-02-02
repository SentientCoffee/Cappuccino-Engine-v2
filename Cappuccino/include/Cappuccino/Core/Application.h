#pragma once

#include "Cappuccino/Core/Window.h"

#include "Cappuccino/Events/Event.h"
#include "Cappuccino/Events/WindowEvents.h"

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

		Window* getWindow() const;

		static Application* getInstance();

		
	private:

		bool onWindowClosed(WindowClosedEvent& e);
		bool onWindowResized(WindowResizedEvent& e);
		
		Window* _window;

		bool _isRunning = true;
		bool _isMinimized = false;
		
		static Application* _instance;
		
	};

	// To be defined in the client app
	Application* createApp();
}