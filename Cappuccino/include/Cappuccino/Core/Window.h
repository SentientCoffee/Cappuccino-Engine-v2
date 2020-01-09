#pragma once

#include "Cappuccino/Events/Event.h"

#include <glfw/glfw3.h>

#include <functional>
#include <string>

namespace Capp {

	struct WindowProperties {
		using EventCallbackFunc = std::function<void(Event&)>;

		std::string title;
		unsigned int width, height;

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

		void update();

		unsigned getWidth() const;
		unsigned getHeight() const;

		void setVSyncEnabled(bool enabled);
		bool isVSyncEnabled() const;

	private:

		void setGLFWCallbacks();
		static void glfwErrorCallback(int errorCode, const char* log);

		inline static bool glfwInitialized = false;
		GLFWwindow* _window;
		WindowProperties _properties;

	};
}
