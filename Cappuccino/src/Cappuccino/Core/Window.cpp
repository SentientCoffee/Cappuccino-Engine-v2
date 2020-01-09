#include "CappPCH.h"
#include "Cappuccino/Core/Window.h"

#include "Cappuccino/Events/KeyEvents.h"
#include "Cappuccino/Events/MouseEvents.h"
#include "Cappuccino/Events/WindowEvents.h"

#include <glad/glad.h>

using namespace Capp;

Window::Window() :
	Window({"Failed to load properly!",
	        100, 100,
	        true, nullptr}) {}

Window::Window(const WindowProperties& properties) :
	_properties(properties) {

	CAPP_PRINT("Creating window \"{0}\" ({1} x {2})", _properties.title.c_str(), _properties.width, _properties.height);
	
	if(!glfwInitialized) {
		const int glfwStatus = glfwInit();
		CAPP_ASSERT(glfwStatus, "Could not initialize GLFW!");
		glfwSetErrorCallback(glfwErrorCallback);
		glfwInitialized = true;
	}
	
	_window = glfwCreateWindow(static_cast<int>(_properties.width),
	                           static_cast<int>(_properties.height),
	                           _properties.title.c_str(),
	                           nullptr, nullptr);
	
	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, &_properties);
	setVSyncEnabled(true);
	setGLFWCallbacks();

	const int gladStatus = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	CAPP_ASSERT(gladStatus);

	CAPP_PRINT_INFO("OpenGL version {0}", glGetString(GL_VERSION));
	CAPP_PRINT_INFO("GLSL version {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
	CAPP_PRINT_INFO("Using {0} {1}", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
}


Window::~Window() {
	glfwDestroyWindow(_window);
}

GLFWwindow* Window::getGlfwWindow() const { return _window; }

void Window::update() {
	
	// Swap the buffers and poll events for the next frame
	glfwPollEvents();
	glfwSwapBuffers(_window);
}

unsigned Window::getWidth() const { return _properties.width; }
unsigned Window::getHeight() const { return _properties.height; }

void Window::setVSyncEnabled(const bool enabled) {
	_properties.isVSyncEnabled = enabled;
	if(enabled) { glfwSwapInterval(1); }
	else { glfwSwapInterval(0); }
}
bool Window::isVSyncEnabled() const { return _properties.isVSyncEnabled; }

void Window::setGLFWCallbacks() {
	// Window resize callback
	glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, const int width, const int height) {
		const auto data = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));
		data->width = width;
		data->height = height;

		WindowResizedEvent e(width, height);
		data->eventFunc(e);
	});

	// Window close callback
	glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
		const auto data = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		WindowClosedEvent e;
		data->eventFunc(e);
	});

	// Window key callback
	glfwSetKeyCallback(_window, [](GLFWwindow* window, const int key, const int scanCode, const int action, const int mods) {
		const auto data = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));
		static int repeatCount = 0;

		switch(action) {
			case GLFW_PRESS: {
				KeyPressedEvent keyPressed(key, 0);
				data->eventFunc(keyPressed);
				break;
			}
			case GLFW_RELEASE: {
				repeatCount = 0;
				KeyReleasedEvent keyReleased(key);
				data->eventFunc(keyReleased);
				break;
			}
			case GLFW_REPEAT: {
				KeyPressedEvent keyRepeat(key, ++repeatCount);
				data->eventFunc(keyRepeat);
				break;
			}
			default: break;
		}
	});

	glfwSetCharCallback(_window, [](GLFWwindow* window, const unsigned int character) {
		const auto data = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		KeyTypedEvent keyTyped(static_cast<int>(character));
		data->eventFunc(keyTyped);
	});

	// Window mouse scrolling callback
	glfwSetScrollCallback(_window, [](GLFWwindow* window, const double xOffset, const double yOffset) {
		const auto data = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		MouseScrolledEvent e(static_cast<float>(xOffset), static_cast<float>(yOffset));
		data->eventFunc(e);
	});

	// Window mouse position callback
	glfwSetCursorPosCallback(_window, [](GLFWwindow* window, const double xPos, const double yPos) {
		const auto data = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		MouseMovedEvent e(static_cast<float>(xPos), static_cast<float>(yPos));
		data->eventFunc(e);
	});

	// Window mouse button callback
	glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, const int button, const int action, const int mods) {
		const auto data = static_cast<WindowProperties*>(glfwGetWindowUserPointer(window));

		switch(action) {
			case GLFW_PRESS: {
				MouseButtonPressedEvent mousePressed(button);
				data->eventFunc(mousePressed);
				break;
			}
			case GLFW_RELEASE: {
				MouseButtonReleasedEvent mouseReleased(button);
				data->eventFunc(mouseReleased);
				break;
			}
			default: break;
		}
	});
}

void Window::glfwErrorCallback(const int errorCode, const char* log) {
	CAPP_PRINT_CRITICAL("GLFW Error! ({0})\n{1}", errorCode, log);
}
