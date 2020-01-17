#include "CappPCH.h"
#include "Cappuccino/Objects/GameObject.h"

using namespace Capp;

unsigned GameObject::objectCount = 0;

GameObject::GameObject() :
	_objectId(objectCount++) {}

GameObject::~GameObject() {
	for(auto model : _models) {
		delete model;
	}
}

bool GameObject::operator==(const GameObject& other) const { return _objectId == other._objectId; }

void GameObject::init() {}
void GameObject::exit() {}
void GameObject::update() {}
void GameObject::onEvent(Event& e) {}

unsigned GameObject::getId() const { return _objectId; }

const std::string& GameObject::getName() const { return _name; }
void GameObject::setName(const std::string& name) { _name = name; }

const std::string& GameObject::getTag() const { return _tag; }
void GameObject::setTag(const std::string& tag) { _tag = tag; }

const glm::vec3& GameObject::getPosition() const { return _transform.getPosition(); }
void GameObject::setPosition(const glm::vec3& position) { _transform.setPosition(position); }
void GameObject::setPosition(const float x, const float y, const float z) { _transform.setPosition(x, y, z); }

const glm::vec3& GameObject::getRotation() const { return _transform.getRotation(); }
void GameObject::setRotation(const glm::vec3& eulerRotation) { _transform.setRotation(eulerRotation); }
void GameObject::setRotation(const float x, const float y, const float z) { _transform.setRotation(x, y, z); }

const glm::vec3& GameObject::getScale() const { return _transform.getScale(); }
void GameObject::setScale(const glm::vec3& scale) { _transform.setScale(scale); }
void GameObject::setScale(const float x, const float y, const float z) { _transform.setScale(x, y, z); }
void GameObject::setScale(const float scale) { _transform.setScale(scale); }
