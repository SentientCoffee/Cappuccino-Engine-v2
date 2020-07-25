#pragma once

#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Core/Window.h"

#include "Cappuccino/Events/Event.h"
#include "Cappuccino/Events/WindowEvents.h"

namespace Capp {

	class Application {
	public:

		Application();
		Application(uint32_t width, uint32_t height, std::string&& title);
		virtual ~Application();

		void run();
		void onEvent(Event& e);

		const Ref<Window>& getWindow() const;

		static Application* getInstance();

		
	private:

		bool onWindowClosed(WindowClosedEvent& e);
		bool onWindowResized(WindowResizedEvent& e);
		
		Ref<Window> _window;

		bool _isRunning = true;
		bool _isMinimized = false;
		
		static Application* _instance;
		
	};

	// To be defined in the client app
	Application* createApp();
}
