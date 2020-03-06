#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Light.h"

#define SHADOW_RESOLUTION 1024

using namespace Capp;

// --------------------------------------------------------------------------
// ----- Light --------------------------------------------------------------
// --------------------------------------------------------------------------


Light::Light(const glm::vec3& colour) :
	_colour(colour)
{
	_shadowBuffer = new Framebuffer(SHADOW_RESOLUTION, SHADOW_RESOLUTION);
	const Attachment depth = { AttachmentType::Texture, InternalFormat::Depth32F,{ WrapMode::ClampToBorder, MinFilter::Nearest } };
	_shadowBuffer->addAttachment(AttachmentTarget::Depth, depth);
}

Light::~Light() {
	delete _shadowBuffer;
}

Framebuffer* Light::getShadowBuffer() const { return _shadowBuffer; }

void Light::setProjection(const glm::mat4& projection) { _shadowProjectionMatrix = projection; }
const glm::mat4& Light::getProjectionMatrix() const { return _shadowProjectionMatrix; }
const Transform& Light::getTransform() const { return _transform; }


// --------------------------------------------------------------------------
// ----- Directional light --------------------------------------------------
// --------------------------------------------------------------------------

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& colour) :
	Light(colour)
{
	_transform.setRotation(direction);
	_shadowBuffer->setName("Shadow Buffer (Directional light)");
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
	Light(colour), _attenuation(attenuation)
{
	_transform.setPosition(position);
	_shadowBuffer->setName("Shadow Buffer (Point light)");
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
	Light(colour), _attenuation(attenuation), _innerCutoffAngle(innerCutoffAngle), _outerCutoffAngle(outerCutoffAngle)
{
	_transform.setPosition(position).setRotation(direction);
	_shadowBuffer->setName("Shadow Buffer (Spotlight)");
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
