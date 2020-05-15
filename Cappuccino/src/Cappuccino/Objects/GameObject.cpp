#include "CappPCH.h"
#include "Cappuccino/Objects/GameObject.h"

#include "Cappuccino/Scenes/SceneManager.h"

using namespace Capp;

unsigned GameObject::_objectCount = 0;

GameObject::GameObject() :
	_objectId(_objectCount++)
{
	SceneManager::getCurrentScene()->addGameObject(this);
	_models.reserve(10);
}

GameObject& GameObject::setPosition(const glm::vec3& position) {
	_rigidbody.setPosition(position);
	for(const auto& model : _models) {
		model->getTransform().setParentTransform(_rigidbody.getTransform().getWorldTransform());
	}
	return *this;
}

GameObject& GameObject::translateBy(const glm::vec3& translation) {
	_rigidbody.setPosition(_rigidbody.getPosition() + translation);
	for(const auto& model : _models) {
		model->getTransform().setParentTransform(_rigidbody.getTransform().getWorldTransform());
	}
	return *this;
}

GameObject& GameObject::setRotation(const glm::vec3& degrees) {
	_rigidbody.setRotation(degrees);
	for(const auto& model : _models) {
		model->getTransform().setParentTransform(_rigidbody.getTransform().getWorldTransform());
	}
	return *this;
}

GameObject& GameObject::rotateBy(const glm::vec3& degrees) {
	_rigidbody.setRotation(_rigidbody.getRotation() + glm::radians(degrees));
	for(const auto& model : _models) {
		model->getTransform().setParentTransform(_rigidbody.getTransform().getWorldTransform());
	}
	return *this;
}

GameObject& GameObject::setScale(const glm::vec3& scale) {
	_rigidbody.setScale(scale);
	for(const auto& model : _models) {
		model->getTransform().setParentTransform(_rigidbody.getTransform().getWorldTransform());
	}
	return *this;
}
GameObject& GameObject::scaleBy(const glm::vec3& scale) {
	_rigidbody.setScale(_rigidbody.getScale() + scale);
	for(const auto& model : _models) {
		model->getTransform().setParentTransform(_rigidbody.getTransform().getWorldTransform());
	}
	return *this;
}

const glm::vec3& GameObject::getPosition() const { return _rigidbody.getPosition(); }
GameObject& GameObject::setPosition(const float x, const float y, const float z) { return setPosition({ x, y, z }); }
GameObject& GameObject::translateBy(const float x, const float y, const float z) { return translateBy({ x, y, z }); }

const glm::vec3& GameObject::getRotation() const { return _rigidbody.getRotation(); }
GameObject& GameObject::setRotation(const float x, const float y, const float z) { return setRotation({ x, y, z }); }
GameObject& GameObject::rotateBy(const float x, const float y, const float z) { return rotateBy({ x, y, z }); }

const glm::vec3& GameObject::getScale() const { return _rigidbody.getScale(); }
GameObject& GameObject::setScale(const float x, const float y, const float z) { return setScale({ x, y, z }); }
GameObject& GameObject::setScale(const float scale) { return setScale({ scale, scale, scale }); }
GameObject& GameObject::scaleBy(const float x, const float y, const float z) { return scaleBy({ x, y, z }); }
GameObject& GameObject::scaleBy(const float scale) { return scaleBy({ scale, scale, scale }); }

bool GameObject::checkCollision(const Ref<GameObject> other) { return _rigidbody.checkCollision(other->_rigidbody); }