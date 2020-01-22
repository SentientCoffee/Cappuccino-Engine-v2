#include "CappPCH.h"
#include "Cappuccino/Rendering/Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
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
	_projectionMatrix = glm::ortho(left, right, bottom, top, -0.1f, 1000.0f);
	_viewMatrix = glm::mat4(1.0f);
	_viewProjection = _projectionMatrix * _viewMatrix;
}

void OrthographicCamera::setProjection(const float left, const float right, const float top, const float bottom) {
	_projectionMatrix = glm::ortho(left, right, bottom, top, -0.1f, 1000.0f);
	_viewProjection = _projectionMatrix * _viewMatrix;
}

const glm::vec3& OrthographicCamera::getPosition() const { return _transform.getPosition(); }
Transform& OrthographicCamera::setPosition(const glm::vec3& position) {
	_transform.setPosition(position);
	viewMatrixCalc();
	return _transform;
}
Transform& OrthographicCamera::setPosition(const float x, const float y, const float z) { return setPosition({ x, y, z }); }

bool OrthographicCamera::isRotatable() const { return _isRotatable; }
void OrthographicCamera::setRotatable(const bool rotatable) { _isRotatable = rotatable; }

float OrthographicCamera::getRotation() const { return _transform.getRotation().z; }
Transform& OrthographicCamera::setRotation(const float rotation) {
	CAPP_ASSERT(_isRotatable, "Camera must be set as rotatable before trying to rotate!");
	_transform.setRotation(0.0f, 0.0f, rotation);
	viewMatrixCalc();
	return _transform;
}

void OrthographicCamera::viewMatrixCalc() {
	_viewMatrix = glm::inverse(_transform.getLocalTransform());
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
	_viewMatrix = glm::lookAt(_transform.getPosition(), target, up);
	_viewProjection = _projectionMatrix * _viewMatrix;
}
void PerspectiveCamera::lookAt(const float targetX, const float targetY, const float targetZ, const glm::vec3& up) { lookAt({ targetX, targetY, targetZ }, up); }

glm::vec3 PerspectiveCamera::getForward() const { return { -_backX, -_backY, -_backZ }; }
glm::vec3 PerspectiveCamera::getUp() const { return { -_upX, -_upY, -_upZ }; }
glm::vec3 PerspectiveCamera::getRight() const { return { -_leftX, -_leftY, -_leftZ }; }

const glm::vec3& PerspectiveCamera::getPosition() const { return _transform.getPosition(); }
Transform& PerspectiveCamera::setPosition(const glm::vec3& position) {
	_transform.setPosition(position);
	viewMatrixCalc();
	return _transform;
}
Transform& PerspectiveCamera::setPosition(const float x, const float y, const float z) { return setPosition({ x, y ,z }); }

const glm::vec3& PerspectiveCamera::getRotation() const { return _transform.getRotation(); }
Transform& PerspectiveCamera::setRotation(const glm::vec3& rotation) {
	_transform.setRotation(rotation);
	viewMatrixCalc();
	return _transform;
}
Transform& PerspectiveCamera::setRotation(const float x, const float y, const float z) { return setRotation({ x, y, z }); }

void PerspectiveCamera::viewMatrixCalc() {
	_viewMatrix = glm::inverse(_transform.getLocalTransform());
	_viewProjection = _projectionMatrix * _viewMatrix;
}
