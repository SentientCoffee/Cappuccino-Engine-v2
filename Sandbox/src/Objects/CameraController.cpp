#include "Objects/CameraController.h"

CameraController::CameraController() :
	CameraController(Capp::Application::getInstance()->getWindow()->getWidth(), Capp::Application::getInstance()->getWindow()->getHeight()) {}

CameraController::CameraController(const unsigned width, const unsigned height) :
	_camera(60.0f, width, height) {
	_camera.setPosition(7.5f, 4.5f, 2.5f);
	_camera.lookAt(0.0f, 0.0f, 0.0f);
}

const Capp::PerspectiveCamera& CameraController::getCamera() const { return _camera; }

void CameraController::update() {
	{
		auto movement = glm::vec3(0.0f);
		const float speed = _camTranslationSpeed * Capp::Time::getDeltaTime();
		
		if(Capp::Input::keyPressed(Capp::KeyCode::W)) {
			movement.z -= speed;
		}
		if(Capp::Input::keyPressed(Capp::KeyCode::S)) {
			movement.z += speed;
		}
		if(Capp::Input::keyPressed(Capp::KeyCode::A)) {
			movement.x -= speed;
		}
		if(Capp::Input::keyPressed(Capp::KeyCode::D)) {
			movement.x += speed;
		}
		if(Capp::Input::keyPressed(Capp::KeyCode::Z)) {
			movement.y += speed;
		}
		if(Capp::Input::keyPressed(Capp::KeyCode::X)) {
			movement.y -= speed;
		}
		
		_camera.setLocalPosition(movement);
	}
	
	if(_mouseHidden) {
		const glm::vec2 mousePos = Capp::Input::getMousePosition();
		const glm::vec2 offset = { mousePos.x - _lastPos.x, _lastPos.y - mousePos.y }; // y is reversed since y-coordinates range from bottom to top
	
		_yaw   += offset.x * _mouseSensitivity * Capp::Time::getDeltaTime();
		_pitch += offset.y * _mouseSensitivity * Capp::Time::getDeltaTime();
		_pitch = glm::clamp(_pitch, -89.0f, 89.0f);

		glm::vec3 rotation;
		rotation.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		rotation.y = sin(glm::radians(_pitch));
		rotation.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
		_lastPos = mousePos;
		
		_camera.lookAt(_camera.getPosition() + glm::normalize(rotation));
	}
}

void CameraController::onEvent(Capp::Event& e) {
	Capp::EventDispatcher d(e);
	d.dispatchEventType<Capp::WindowResizedEvent>(BIND_EVENT_FN(CameraController::onWindowResized));
	d.dispatchEventType<Capp::KeyPressedEvent>(BIND_EVENT_FN(CameraController::onKeyPressed));
}

bool CameraController::onWindowResized(Capp::WindowResizedEvent& e) {
	if(e.getWidth() <= 0 || e.getHeight() <= 0) {
		return false;
	}
	
	_camera.setProjection(60.0f, e.getWidth(), e.getHeight());
	return false;
}

bool CameraController::onKeyPressed(Capp::KeyPressedEvent& e) {
	const auto window = Capp::Application::getInstance()->getWindow();
	
	if(e.getKeyCode() == Capp::KeyCode::Escape) {
		_mouseHidden ^= 1;
	}

	if(!_mouseHidden) {
		window->showMouseCursor();
	}
	else {
		window->disableMouseCursor();
		_lastPos = Capp::Input::getMousePosition();
	}
	
	return false;
}