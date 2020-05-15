#include "CappPCH.h"
#include "Cappuccino/Core/Window.h"

#include "Cappuccino/Events/KeyEvents.h"
#include "Cappuccino/Events/MouseEvents.h"
#include "Cappuccino/Events/WindowEvents.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

using namespace Capp;

static void glfwErrorCallback(const int errorCode, const char* log) {
	CAPP_PRINT_CRITICAL("GLFW Error! ({0})\n{1}", errorCode, log);
}

static void glErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	// ignore non-significant error/warning codes
	if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	char buffer[9] = { '\0' };
	sprintf(buffer, "%.8x", id);

	std::string log = "OpenGL(0x" + std::string(buffer) + "): ";

	switch(type) {
		case GL_DEBUG_TYPE_ERROR:					log += "ERROR"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:		log += "DEPRECATED BEHAVIOUR"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:		log += "UNDEFINED BEHAVIOUR"; break;
		case GL_DEBUG_TYPE_PORTABILITY:				log += "PORTABILITY ISSUE"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:				log += "PERFORMANCE ISSUE"; break;
		case GL_DEBUG_TYPE_MARKER:					log += "TYPE MARKER"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:				log += "PUSH GROUP"; break;
		case GL_DEBUG_TYPE_POP_GROUP:				log += "POP GROUP"; break;
		case GL_DEBUG_TYPE_OTHER:					log += "OTHER"; break;
		default: break;
	}

	log += "\n\tSOURCE: ";
	switch(source) {
		case GL_DEBUG_SOURCE_API:					log += "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:			log += "Window system"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:		log += "Shader compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:			log += "Third party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:			log += "Application"; break;
		case GL_DEBUG_SOURCE_OTHER:					log += "Other"; break;
		default: break;
	}

	log += " \n\tSEVERITY: ";
	switch(severity) {
		case GL_DEBUG_SEVERITY_HIGH:				log += "HIGH"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:				log += "MEDIUM"; break;
		case GL_DEBUG_SEVERITY_LOW:					log += "LOW"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:		log += "NOTIFICATION"; break;
		default: break;
	}

	log += "\n" + std::string(message);
	
	if(type == GL_DEBUG_TYPE_ERROR) {
		CAPP_ASSERT(type != GL_DEBUG_TYPE_ERROR, "{0}", log);
	}
	else if(severity == GL_DEBUG_SEVERITY_HIGH) {
		CAPP_PRINT_ERROR("{0}", log);
	}
	else if(severity == GL_DEBUG_SEVERITY_MEDIUM) {
		CAPP_PRINT_WARNING("{0}", log);
	}
	else if(severity == GL_DEBUG_SEVERITY_LOW) {
		CAPP_PRINT_INFO("{0}", log);
	}
	else {
		CAPP_PRINT("{0}", log);
	}
}

Window::Window() :
	Window({"Failed to load properly!",
		100, 100,
		true, nullptr}) {}

Window::Window(const WindowProperties& properties) :
	_properties(properties) {

	CAPP_PRINT_INFO("Creating window \"{0}\" ({1} x {2})", _properties.title, _properties.width, _properties.height);
	
	if(!glfwInitialized) {
		const int glfwStatus = glfwInit();
		CAPP_ASSERT(glfwStatus, "Could not initialize GLFW!");
		glfwSetErrorCallback(glfwErrorCallback);
		glfwInitialized = true;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	_window = glfwCreateWindow(static_cast<int>(_properties.width),
		static_cast<int>(_properties.height),
		_properties.title.c_str(),
		nullptr, nullptr);
	
	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, &_properties);
	setVSyncEnabled(_properties.isVSyncEnabled);
	setGLFWCallbacks();

	const int gladStatus = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
	CAPP_ASSERT(gladStatus, "Could not initialize GLAD!");

	#if CAPP_DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glErrorCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	#endif
	
	CAPP_PRINT_INFO("OpenGL version {0}", glGetString(GL_VERSION));
	CAPP_PRINT_INFO("GLSL version {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
	CAPP_PRINT_INFO("Using {0} {1}"
		"\n----------------------------------------------------------------------------------------------------",
		glGetString(GL_VENDOR), glGetString(GL_RENDERER));
}


Window::~Window() {
	glfwDestroyWindow(_window);
}

GLFWwindow* Window::getGlfwWindow() const { return _window; }

void Window::update() const {
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

void Window::showMouseCursor() const { glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }
void Window::hideMouseCursor() const { glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); }
void Window::disableMouseCursor() const { glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }

void Window::setGLFWCallbacks() const {
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
