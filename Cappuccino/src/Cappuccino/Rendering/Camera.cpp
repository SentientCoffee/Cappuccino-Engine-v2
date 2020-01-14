#include "CappPCH.h"
#include "Cappuccino/Rendering/Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace Capp;

// -------------------------------------------------------------
// ----- Camera ------------------------------------------------
// -------------------------------------------------------------

const glm::mat4& Camera::getProjectionMatrix() const { return _projectionMatrix; }
const glm::mat4& Camera::getViewMatrix() const { return _viewMatrix; }
const glm::mat4& Camera::getViewProjection() const { return _viewProjection; }

// -------------------------------------------------------------
// ----- Orthographic Camera -----------------------------------
// -------------------------------------------------------------

OrthographicCamera::OrthographicCamera(const float left, const float right, const float top, const float bottom) {
	_projectionMatrix = glm::ortho(left, right, bottom, top, -0.1f, 100.0f);
	_viewMatrix = glm::mat4(1.0f);
	_viewProjection = _projectionMatrix * _viewMatrix;
}

void OrthographicCamera::setProjection(const float left, const float right, const float top, const float bottom) {
	_projectionMatrix = glm::ortho(left, right, bottom, top, -0.1f, 100.0f);
	_viewProjection = _projectionMatrix * _viewMatrix;
}

const glm::vec3& OrthographicCamera::getPosition() const { return _position; }
void OrthographicCamera::setPosition(const glm::vec3& position) {
	_position = position;
	viewMatrixCalc();
}
void OrthographicCamera::setPosition(const float x, const float y, const float z) { setPosition({ x, y, z }); }

bool OrthographicCamera::isRotatable() const { return _isRotatable; }
void OrthographicCamera::setRotatable(const bool rotatable) { _isRotatable = rotatable; }

float OrthographicCamera::getRotation() const { return _rotation; }
void OrthographicCamera::setRotation(const float rotation) {
	_rotation = rotation;
	viewMatrixCalc();
}

void OrthographicCamera::viewMatrixCalc() {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position);
	if(_isRotatable) {
		transform = glm::rotate(transform, glm::radians(_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else {
		CAPP_ASSERT(_rotation != 0.0f, "Camera must be set as rotatable before trying to rotate!");
	}
	
	_viewMatrix = glm::inverse(transform);
	_viewProjection = _projectionMatrix * _viewMatrix;
}


// -------------------------------------------------------------
// ----- Perspective Camera ------------------------------------
// -------------------------------------------------------------

PerspectiveCamera::PerspectiveCamera(const float fovY, const unsigned width, const unsigned height) {
	_projectionMatrix = glm::perspective(glm::radians(fovY), static_cast<float>(width) / static_cast<float>(height), 0.01f, 100.0f);
	_viewMatrix = glm::mat4(1.0f);
	_viewProjection = _projectionMatrix * _viewMatrix;
}

void PerspectiveCamera::setProjection(const float fovY, const unsigned width, const unsigned height) {
	_projectionMatrix = glm::perspective(glm::radians(fovY), static_cast<float>(width) / static_cast<float>(height), 0.01f, 100.0f);
	_viewProjection = _projectionMatrix * _viewMatrix;
}

void PerspectiveCamera::lookAt(const glm::vec3& target, const glm::vec3& up) {
	_viewMatrix = glm::lookAt(_position, target, up);
	_viewProjection = _projectionMatrix * _viewMatrix;
}
void PerspectiveCamera::lookAt(const float targetX, const float targetY, const float targetZ, const glm::vec3& up) { lookAt({ targetX, targetY, targetZ }, up); }

glm::vec3 PerspectiveCamera::getForward() const { return { -_backX, -_backY, -_backZ }; }
glm::vec3 PerspectiveCamera::getUp() const { return { -_upX, -_upY, -_upZ }; }
glm::vec3 PerspectiveCamera::getRight() const { return { -_leftX, -_leftY, -_leftZ }; }

const glm::vec3& PerspectiveCamera::getPosition() const { return _position; }
void PerspectiveCamera::setPosition(const glm::vec3& position) {
	_position = position;
	viewMatrixCalc();
}
void PerspectiveCamera::setPosition(const float x, const float y, const float z) { setPosition({ x, y ,z }); }

const glm::vec3& PerspectiveCamera::getRotation() const { return _rotation; }
void PerspectiveCamera::setRotation(const glm::vec3& rotation) {
	_rotation = rotation;
	viewMatrixCalc();
}
void PerspectiveCamera::setRotation(const float x, const float y, const float z) { setRotation({ x, y, z }); }

void PerspectiveCamera::viewMatrixCalc() {
	const glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) *
			glm::mat4_cast(glm::quat(_rotation));
	
	_viewMatrix = glm::inverse(transform);
	_viewProjection = _projectionMatrix * _viewMatrix;
}
