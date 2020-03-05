#pragma once
#include <Cappuccino.h>

class CameraController {
public:

	CameraController();
	CameraController(unsigned width, unsigned height);
	~CameraController() = default;

	const Capp::PerspectiveCamera& getCamera() const;

	void update();
	void onEvent(Capp::Event& e);

private:

	bool onWindowResized(Capp::WindowResizedEvent& e);
	bool onKeyPressed(Capp::KeyPressedEvent& e);
	
	Capp::PerspectiveCamera _camera;
	
	float _camTranslationSpeed = 5.0f;
	float _mouseSensitivity = 5.0f;

	glm::vec2 _lastPos = { 0.0, 0.0f };

	float _yaw = -90.0f;
	float _pitch = 0.0f;

	bool _mouseHidden = true;
};
