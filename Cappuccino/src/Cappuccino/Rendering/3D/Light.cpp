#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Light.h"

using namespace Capp;

// --------------------------------------------------------------------------
// ----- Directional light --------------------------------------------------
// --------------------------------------------------------------------------

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& colour) :
	_colour(colour) {
	_transform.setRotation(direction);
}

const glm::vec3& DirectionalLight::getDirection() const { return _transform.getRotation(); }
DirectionalLight& DirectionalLight::setDirection(const glm::vec3& direction) {
	_transform.setRotation(direction);
	return *this;
}
DirectionalLight& DirectionalLight::setDirection(const float x, const float y, const float z) { return setDirection({ x, y, z }); }

const glm::vec3& DirectionalLight::getColour() const { return _colour; }
DirectionalLight& DirectionalLight::setColour(const glm::vec3& colour) {
	_colour = colour;
	return *this;
}
DirectionalLight& DirectionalLight::setColour(const float r, const float g, const float b) { return setColour({ r, g, b }); }

// --------------------------------------------------------------------------
// ----- Point light --------------------------------------------------------
// --------------------------------------------------------------------------

PointLight::PointLight(const glm::vec3& position, const glm::vec3& colour, const float attenuation) :
	_colour(colour), _attenuation(attenuation) {
	_transform.setPosition(position);
}

const glm::vec3& PointLight::getPosition() const { return _transform.getPosition(); }
PointLight& PointLight::setPosition(const glm::vec3& position) {
	_transform.setPosition(position);
	return *this;
}
PointLight& PointLight::setPosition(const float x, const float y, const float z) { return setPosition({ x, y, z }); }

const glm::vec3& PointLight::getColour() const { return _colour; }
PointLight& PointLight::setColour(const glm::vec3& colour) {
	_colour = colour;
	return *this;
}
PointLight& PointLight::setColour(const float r, const float g, const float b) { return setColour({ r, g, b }); }

float PointLight::getAttenuation() const { return _attenuation; }
PointLight& PointLight::setAttenuation(const float attenuation) {
	_attenuation = attenuation;
	return *this;
}

// --------------------------------------------------------------------------
// ----- Spotlight ----------------------------------------------------------
// --------------------------------------------------------------------------

Spotlight::Spotlight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour, const float attenuation, const float innerCutoffAngle, const float outerCutoffAngle) :
	_colour(colour), _attenuation(attenuation), _innerCutoffAngle(innerCutoffAngle), _outerCutoffAngle(outerCutoffAngle) {
	_transform.setPosition(position).setRotation(direction);
}

const glm::vec3& Spotlight::getPosition() const { return _transform.getPosition(); }
Spotlight& Spotlight::setPosition(const glm::vec3& position) {
	_transform.setPosition(position);
	return *this;
}
Spotlight& Spotlight::setPosition(const float x, const float y, const float z) { return setPosition({ x, y, z }); }

const glm::vec3& Spotlight::getDirection() const { return _transform.getRotation(); }
Spotlight& Spotlight::setDirection(const glm::vec3& direction) {
	_transform.setRotation(direction);
	return *this;
}
Spotlight& Spotlight::setDirection(const float x, const float y, const float z) { return setDirection({ x, y, z }); }

const glm::vec3& Spotlight::getColour() const { return _colour; }
Spotlight& Spotlight::setColour(const glm::vec3& colour) {
	_colour = colour;
	return *this;
}
Spotlight& Spotlight::setColour(const float r, const float g,const float b) { return setColour({ r, g, b }); }

float Spotlight::getAttenuation() const { return _attenuation; }
Spotlight& Spotlight::setAttenuation(const float attenuation) {
	_attenuation = attenuation;
	return *this;
}

float Spotlight::getInnerCutoffAngle() const { return _innerCutoffAngle; }
Spotlight& Spotlight::setInnerCutoffAngle(const float angle) {
	_innerCutoffAngle = angle;
	return *this;
}

float Spotlight::getOuterCutoffAngle() const { return _outerCutoffAngle; }
Spotlight& Spotlight::setOuterCutoffAngle(const float angle) {
	_outerCutoffAngle = angle;
	return *this;
}
