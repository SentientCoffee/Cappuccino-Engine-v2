#pragma once

#include "Cappuccino/Events/Event.h"

#include <functional>
#include <string>

struct GLFWwindow;

namespace Capp {

	struct WindowProperties {
		using EventCallbackFunc = std::function<void(Event&)>;

		std::string title;
		uint32_t width, height;

		bool isVSyncEnabled;
		EventCallbackFunc eventFunc;
	};

	class Window {
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		Window();
		Window(const WindowProperties& properties);
		~Window();

		GLFWwindow* getGlfwWindow() const;

		void update() const;

		uint32_t getWidth() const;
		uint32_t getHeight() const;

		void setVSyncEnabled(bool enabled);
		bool isVSyncEnabled() const;

		void showMouseCursor() const;
		void hideMouseCursor() const;
		void disableMouseCursor() const;

	private:

		void setGLFWCallbacks() const;

		inline static bool glfwInitialized = false;
		GLFWwindow* _window;
		WindowProperties _properties;

	};
}
