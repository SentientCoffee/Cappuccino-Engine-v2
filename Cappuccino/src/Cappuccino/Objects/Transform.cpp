#include "CappPCH.h"
#include "Cappuccino/Objects/Transform.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

using namespace Capp;

const glm::mat4& Transform::getLocalTransform() const {
	calculateTransform();
	return _localTransform;
}
const glm::mat4& Transform::getWorldTransform() const {
	calculateTransform();
	return _worldTransform;
}

const glm::vec3& Transform::getPosition() const { return _position; }
Transform& Transform::setPosition(const glm::vec3& position) {
	_position = position;
	_transformChanged = true;
	return *this;
}
Transform& Transform::setPosition(const float x, const float y, const float z) {
	return setPosition({ x, y, z });
}

const glm::vec3& Transform::getRotation() const { return _rotation; }
Transform& Transform::setRotation(const glm::vec3& degrees) {
	_rotation = degrees;
	_transformChanged = true;
	return *this;
}
Transform& Transform::setRotation(const float x, const float y, const float z) {
	return setRotation({ x, y, z });
}

const glm::vec3& Transform::getScale() const { return _scale; }
Transform& Transform::setScale(const glm::vec3& scale) {
	_scale = scale;
	_transformChanged = true;
	return *this;
}
Transform& Transform::setScale(const float x, const float y, const float z) {
	return setScale({ x, y, z });
}
Transform& Transform::setScale(const float scale) {
	return setScale({ scale, scale, scale });
}

const glm::mat4& Transform::getParentTransform() const { return _parentTransform; }
void Transform::setParentTransform(const glm::mat4& parent) const {
	_transformChanged = true;
	_parentTransform = parent;
}

void Transform::calculateTransform() const {
	if(!_transformChanged) {
		return;
	}

	_localTransform = glm::translate(glm::mat4(1.0f), _position) *
		glm::mat4_cast(glm::quat(glm::radians(_rotation))) *
		glm::scale(glm::mat4(1.0f), _scale);

	_worldTransform = _parentTransform * _localTransform;

	_transformChanged = false;
}
