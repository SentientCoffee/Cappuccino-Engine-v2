#include "CappPCH.h"
#include "Cappuccino/Rendering/Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace Capp;

// -------------------------------------------------------------
// ----- Camera ------------------------------------------------
// -------------------------------------------------------------

const glm::mat4& Camera::getProjectionMatrix() const { return _projectionMatrix; }
const glm::mat4& Camera::getViewMatrix() const { return _viewMatrix; }
const glm::mat4& Camera::getViewProjection() const { return _viewProjection; }

void Camera::viewMatrixCalc() {
	_viewMatrix = glm::inverse(_transform.getLocalTransform());
	_viewProjection = _projectionMatrix * _viewMatrix;
}


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
OrthographicCamera& OrthographicCamera::setPosition(const glm::vec3& position) {
	_transform.setPosition(position);
	viewMatrixCalc();
	return *this;
}
OrthographicCamera& OrthographicCamera::setPosition(const float x, const float y, const float z) { return setPosition({ x, y, z }); }

bool OrthographicCamera::isRotatable() const { return _isRotatable; }
void OrthographicCamera::setRotatable(const bool rotatable) { _isRotatable = rotatable; }

float OrthographicCamera::getRotation() const { return _transform.getRotation().z; }
OrthographicCamera& OrthographicCamera::setRotation(const float rotation) {
	CAPP_ASSERT(_isRotatable, "Camera must be set as rotatable before trying to rotate!");
	_transform.setRotation(0.0f, 0.0f, rotation);
	viewMatrixCalc();
	return *this;
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
	
	glm::vec3 rotation;
	glm::extractEulerAngleXYZ(_viewMatrix, rotation.x, rotation.y, rotation.z);
	_transform.setRotation(glm::degrees(rotation));
}
void PerspectiveCamera::lookAt(const float targetX, const float targetY, const float targetZ, const glm::vec3& up) { lookAt({ targetX, targetY, targetZ }, up); }

glm::vec3 PerspectiveCamera::getForward() const { return { -_backX, -_backY, -_backZ }; }
glm::vec3 PerspectiveCamera::getUp() const { return { -_upX, -_upY, -_upZ }; }
glm::vec3 PerspectiveCamera::getRight() const { return { -_leftX, -_leftY, -_leftZ }; }

const glm::vec3& PerspectiveCamera::getPosition() const { return _transform.getPosition(); }
PerspectiveCamera& PerspectiveCamera::setPosition(const glm::vec3& position) {
	_transform.setPosition(position);
	viewMatrixCalc();
	return *this;
}
PerspectiveCamera& PerspectiveCamera::setPosition(const float x, const float y, const float z) { return setPosition({ x, y ,z }); }

PerspectiveCamera& PerspectiveCamera::setLocalPosition(const glm::vec3& localPosition) {
	_viewMatrix[3] -= glm::vec4(localPosition, 0.0f);
	_transform.setPosition(-glm::inverse(glm::mat3(_viewMatrix)) * _viewMatrix[3]);
	return *this;
}

const glm::vec3& PerspectiveCamera::getRotation() const { return _transform.getRotation(); }
PerspectiveCamera& PerspectiveCamera::setRotation(const glm::vec3& rotation) {
	_transform.setRotation(rotation);
	viewMatrixCalc();
	return *this;
}
PerspectiveCamera& PerspectiveCamera::setRotation(const float x, const float y, const float z) { return setRotation({ x, y, z }); }
