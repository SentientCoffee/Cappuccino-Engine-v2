#include "CappPCH.h"
#include "Cappuccino/Objects/Rigidbody.h"

#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

using namespace Capp;

// -------------------------------------------------------------------
// ----- Physics -----------------------------------------------------
// -------------------------------------------------------------------

const float Physics::gravity = -9.81f;
const float Physics::universalG = 6.67f * static_cast<float>(pow(10, -11));

// -------------------------------------------------------------------
// ----- RigidBody ---------------------------------------------------
// -------------------------------------------------------------------

RigidBody::RigidBody(const HitboxVector& hitboxes) :
	_hitboxes(hitboxes) {
	
	for(auto hitbox : _hitboxes) {
		hitbox->getTransform().setParentTransform(_transform.getWorldTransform());
	}
}
RigidBody::RigidBody(const HitboxInitList& hitboxes) :
	RigidBody(HitboxVector(hitboxes)) {}

RigidBody::~RigidBody() {
	for(auto hitbox : _hitboxes) {
		delete hitbox;
	}
	
	_hitboxes.clear();
}

RigidBody& RigidBody::operator=(const HitboxVector& hitboxes) {
	_hitboxes = hitboxes;

	for(auto hitbox : _hitboxes) {
		hitbox->getTransform().setParentTransform(_transform.getWorldTransform());
	}
	
	return *this;
}
RigidBody& RigidBody::operator=(const HitboxInitList& hitboxes) {
	_hitboxes = hitboxes;

	for(auto hitbox : _hitboxes) {
		hitbox->getTransform().setParentTransform(_transform.getWorldTransform());
	}
	
	return *this;
}

bool RigidBody::isMovable() const { return _isMovable; }
void RigidBody::setMovable(const bool movable) { _isMovable = movable; }

bool RigidBody::isTangible() const { return _isTangible; }
void RigidBody::setTangible(const bool tangible) { _isTangible = tangible; }

bool RigidBody::hasGravity() const { return _hasGravity; }
void RigidBody::setGravity(const bool gravity) { _hasGravity = gravity; }

float RigidBody::getMass() const { return _mass; }
void RigidBody::setMass(const float mass) { _mass = mass; }

void RigidBody::setAcceleration(const glm::vec3& acceleration) {
	_acceleration = acceleration;
	addForce({ 0.0f, 0.0f, 0.0f });
}
void RigidBody::addForce(const glm::vec3& force) {
	_acceleration = force / _mass;
	if(glm::length2(_acceleration) > glm::length2(_maxAccel)) {
		_acceleration = _maxAccel;
	}

	addVelocity(_acceleration);
}

void RigidBody::setVelocity(const glm::vec3& velocity) {
	_velocity = velocity;
	addAcceleration({ 0.0f, 0.0f, 0.0f });
}
void RigidBody::addAcceleration(const glm::vec3& acceleration) {
	_velocity += acceleration;
	if(glm::length2(_velocity) > glm::length2(_maxVelocity)) {
		_velocity = _maxVelocity;
	}

	addVelocity(_velocity);
}

void RigidBody::addVelocity(const glm::vec3& velocity) {
	const glm::vec3 position = _transform.getPosition() + velocity;
	setPosition(position);
}

RigidBody& RigidBody::setPosition(const glm::vec3& position) {
	_transform.setPosition(position);
	for(auto hitbox : _hitboxes) {
		hitbox->getTransform().setParentTransform(_transform.getWorldTransform());
	}
	return *this;
}
RigidBody& RigidBody::setPosition(const float x, const float y, const float z) { return setPosition({ x, y, z }); }
const glm::vec3& RigidBody::getPosition() const { return _transform.getPosition(); }

RigidBody& RigidBody::setRotation(const glm::vec3& eulerRotation) {
	_transform.setRotation(eulerRotation);
	for(auto hitbox : _hitboxes) {
		hitbox->getTransform().setParentTransform(_transform.getWorldTransform());
	}
	return *this;
}
RigidBody& RigidBody::setRotation(const float x, const float y, const float z) { return setRotation({ x, y, z }); }
const glm::vec3& RigidBody::getRotation() const { return _transform.getRotation(); }

RigidBody& RigidBody::setScale(const glm::vec3& scale) {
	_transform.setScale(scale);
	for(auto hitbox : _hitboxes) {
		hitbox->getTransform().setParentTransform(_transform.getWorldTransform());
	}
	return *this;	
}
RigidBody& RigidBody::setScale(const float x, const float y, const float z) { return setScale({ x, y, z }); }
RigidBody& RigidBody::setScale(const float scale) { return setScale({ scale, scale, scale }); }
const glm::vec3& RigidBody::getScale() const { return _transform.getScale(); }

Transform& RigidBody::getTransform() { return _transform; }

bool RigidBody::checkCollision(const RigidBody& other) {
	if(_hitboxes.empty() || other._hitboxes.empty()) {
		return false;
	}

	for(auto thisHitbox : _hitboxes) {
		for(auto otherHitbox : other._hitboxes) {

			if(thisHitbox->getHitboxType() == HitboxType::None) {
				CAPP_ASSERT(false, "Hitbox must have a type to check for collisions!");
				continue;
			}
			
			if(thisHitbox->getHitboxType() == HitboxType::Cube) {
				const auto th = dynamic_cast<HitboxCube*>(thisHitbox);
				switch(otherHitbox->getHitboxType()) {
					case HitboxType::Cube: {
						if(th->checkCollision(dynamic_cast<HitboxCube*>(otherHitbox))) {
							return true;
						}
						break;
					}
					case HitboxType::Sphere: {
						if(th->checkCollision(dynamic_cast<HitboxSphere*>(otherHitbox))) {
							return true;
						}
						break;
					}
					case HitboxType::None:
					default: {
						CAPP_ASSERT(false, "Hitbox must have a type to check for collisions!");
						break;
					}
				}
			}

			if(thisHitbox->getHitboxType() == HitboxType::Sphere) {
				const auto th = dynamic_cast<HitboxSphere*>(thisHitbox);
				switch(otherHitbox->getHitboxType()) {
					case HitboxType::Cube: {
						if(th->checkCollision(dynamic_cast<HitboxCube*>(otherHitbox))) {
							return true;
						}
						break;
					}
					case HitboxType::Sphere: {
						if(th->checkCollision(dynamic_cast<HitboxSphere*>(otherHitbox))) {
							return true;
						}
						break;
					}
					case HitboxType::None:
					default: {
						CAPP_ASSERT(false, "Hitbox must have a type to check for collisions!");
						break;
					}
				}
			}
		}
	}

	return false;
}

bool RigidBody::checkCollision(Hitbox* other) {
	if(_hitboxes.empty()) {
		return false;
	}

	for(auto thisHitbox : _hitboxes) {

		if(thisHitbox->getHitboxType() == HitboxType::None) {
			CAPP_ASSERT(false, "Hitbox must have a type to check for collisions!");
			continue;
		}

		if(thisHitbox->getHitboxType() == HitboxType::Cube) {
			const auto th = dynamic_cast<HitboxCube*>(thisHitbox);
			switch(other->getHitboxType()) {
				case HitboxType::Cube: {
					if(th->checkCollision(dynamic_cast<HitboxCube*>(other))) {
						return true;
					}
					break;
				}
				case HitboxType::Sphere: {
					if(th->checkCollision(dynamic_cast<HitboxSphere*>(other))) {
						return true;
					}
					break;
				}
				case HitboxType::None:
				default: {
					CAPP_ASSERT(false, "Hitbox must have a type to check for collisions!");
					break;
				}
			}
		}

		if(thisHitbox->getHitboxType() == HitboxType::Sphere) {
			const auto th = dynamic_cast<HitboxSphere*>(thisHitbox);
			switch(other->getHitboxType()) {
				case HitboxType::Cube: {
					if(th->checkCollision(dynamic_cast<HitboxCube*>(other))) {
						return true;
					}
					break;
				}
				case HitboxType::Sphere: {
					if(th->checkCollision(dynamic_cast<HitboxSphere*>(other))) {
						return true;
					}
					break;
				}
				case HitboxType::None:
				default: {
					CAPP_ASSERT(false, "Hitbox must have a type to check for collisions!");
					break;
				}
			}
		}
	}

	return false;
}

RigidBody::HitboxVector RigidBody::getHitboxes() const { return _hitboxes; }

RigidBody::HitboxIterator RigidBody::begin() { return _hitboxes.begin(); }
RigidBody::HitboxIterator RigidBody::end() { return _hitboxes.end(); }
RigidBody::ConstHitboxIterator RigidBody::begin() const { return _hitboxes.begin(); }
RigidBody::ConstHitboxIterator RigidBody::end() const { return _hitboxes.end(); }

