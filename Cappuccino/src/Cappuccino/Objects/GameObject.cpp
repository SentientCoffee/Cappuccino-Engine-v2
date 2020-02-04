#include "CappPCH.h"
#include "Cappuccino/Objects/GameObject.h"

using namespace Capp;

unsigned GameObject::objectCount = 0;
std::vector<GameObject*> GameObject::gameObjects;

GameObject::GameObject() :
	_objectId(objectCount++) {
	gameObjects.push_back(this);
	_models.reserve(10);
}

GameObject::GameObject(const ModelVector& models) :
	_objectId(objectCount++), _models(models) {
	for(auto model : _models) {
		model->getTransform().setParentTransform(_rigidbody.getTransform().getWorldTransform());
	}
	gameObjects.push_back(this);
}

GameObject::GameObject(const ModelInitList& models) :
	GameObject(ModelVector(models)) {}

GameObject::~GameObject() {
	for(auto model : _models) {
		delete model;
	}
}

bool GameObject::operator==(const GameObject& other) const { return _objectId == other._objectId; }
unsigned GameObject::getId() const { return _objectId; }

const std::string& GameObject::getName() const { return _name; }
void GameObject::setName(const std::string& name) { _name = name; }

const std::string& GameObject::getTag() const { return _tag; }
void GameObject::setTag(const std::string& tag) { _tag = tag; }

bool GameObject::isActive() const { return _isActive; }
void GameObject::setActive(const bool active) { _isActive = active; }

bool GameObject::isVisible() const { return _isVisible; }
void GameObject::setVisible(const bool visible) { _isVisible = visible; }

const glm::vec3& GameObject::getPosition() const { return _rigidbody.getPosition(); }
GameObject& GameObject::setPosition(const glm::vec3& position) {
	_rigidbody.setPosition(position);
	for(auto model : _models) {
		model->getTransform().setParentTransform(_rigidbody.getTransform().getWorldTransform());
	}
	return *this;
}
GameObject& GameObject::setPosition(const float x, const float y, const float z) { return setPosition({ x, y, z }); }
GameObject& GameObject::translateBy(const glm::vec3& translation) {
	_rigidbody.setPosition(_rigidbody.getPosition() + translation);
	for(auto model : _models) {
		model->getTransform().setParentTransform(_rigidbody.getTransform().getWorldTransform());
	}
	return *this;
}
GameObject& GameObject::translateBy(const float x, const float y, const float z) { return translateBy({ x, y, z }); }

const glm::vec3& GameObject::getRotation() const { return _rigidbody.getRotation(); }
GameObject& GameObject::setRotation(const glm::vec3& eulerRotation) {
	_rigidbody.setRotation(eulerRotation);
	for(auto model : _models) {
		model->getTransform().setParentTransform(_rigidbody.getTransform().getWorldTransform());
	}
	return *this;
}
GameObject& GameObject::setRotation(const float x, const float y, const float z) { return setRotation({ x, y, z }); }
GameObject& GameObject::rotateBy(const glm::vec3& eulerRotation) {
	_rigidbody.setRotation(_rigidbody.getRotation() + glm::radians(eulerRotation));
	for(auto model : _models) {
		model->getTransform().setParentTransform(_rigidbody.getTransform().getWorldTransform());
	}
	return *this;
}
GameObject& GameObject::rotateBy(const float x, const float y, const float z) { return rotateBy({ x, y, z }); }

const glm::vec3& GameObject::getScale() const { return _rigidbody.getScale(); }
GameObject& GameObject::setScale(const glm::vec3& scale) {
	_rigidbody.setScale(scale);
	for(auto model : _models) {
		model->getTransform().setParentTransform(_rigidbody.getTransform().getWorldTransform());
	}
	return *this;
}
GameObject& GameObject::setScale(const float x, const float y, const float z) { return setScale({ x, y, z }); }
GameObject& GameObject::setScale(const float scale) { return setScale({ scale, scale, scale }); }
GameObject& GameObject::scaleBy(const glm::vec3& scale) {
	_rigidbody.setScale(_rigidbody.getScale() + scale);
	for(auto model : _models) {
		model->getTransform().setParentTransform(_rigidbody.getTransform().getWorldTransform());
	}
	return *this;
}
GameObject& GameObject::scaleBy(const float x, const float y, const float z) { return scaleBy({ x, y, z }); }
GameObject& GameObject::scaleBy(const float scale) { return scaleBy({ scale, scale, scale }); }

bool GameObject::checkCollision(GameObject* other) {
	return _rigidbody.checkCollision(other->_rigidbody);
}

const RigidBody& GameObject::getRigidBody() const { return _rigidbody; }
GameObject::ModelVector GameObject::getModels() const { return _models; }