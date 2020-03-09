#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Light.h"

#define SHADOW_RESOLUTION 1024u

using namespace Capp;

// --------------------------------------------------------------------------
// ----- Light --------------------------------------------------------------
// --------------------------------------------------------------------------


Light::Light(const glm::vec3& colour) :
	_colour(colour)
{
	_shadowBuffer = new Framebuffer(SHADOW_RESOLUTION, SHADOW_RESOLUTION);
	const Attachment depth = { AttachmentType::Texture, InternalFormat::Depth16,{ WrapMode::ClampToBorder, MinFilter::Nearest, MagFilter::Nearest, Anisotropy::Aniso1x, glm::vec4(1.0f) } };
	_shadowBuffer->addAttachment(AttachmentTarget::Depth, depth);
}

Light::~Light() {
	delete _shadowBuffer;
}

void Light::viewMatrixCalc() {
	_shadowViewMatrix = glm::inverse(_transform.getLocalTransform());
	_shadowViewProjection = _shadowProjectionMatrix * _shadowViewMatrix;
}

Framebuffer* Light::getShadowBuffer() const { return _shadowBuffer; }
void Light::setShadowResolution(const glm::ivec2& resolution) const { _shadowBuffer->resize(resolution.x, resolution.y); }
void Light::setShadowResolution(const unsigned x, const unsigned y) const { _shadowBuffer->resize(x, y); }
void Light::setShadowResolution(const unsigned resolution) const { _shadowBuffer->resize(resolution, resolution); }

void Light::setProjection(const glm::mat4& projection) { _shadowProjectionMatrix = projection; }
const glm::mat4& Light::getProjectionMatrix() const { return _shadowProjectionMatrix; }
const glm::mat4& Light::getViewMatrix() const { return _shadowViewMatrix; }
const glm::mat4& Light::getViewProjection() const { return _shadowViewProjection; }
const Transform& Light::getTransform() const { return _transform; }

// --------------------------------------------------------------------------
// ----- Directional light --------------------------------------------------
// --------------------------------------------------------------------------

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& colour) :
	Light(colour)
{
	const float maxBound = 10.0f;
	setProjection(glm::ortho(-maxBound, maxBound, -maxBound, maxBound, -10.0f, 100.0f));
	setDirection(direction);
	_shadowBuffer->setName("Shadow Buffer (Directional light)");
}

const glm::vec3& DirectionalLight::getDirection() const { return _transform.getRotation(); }
DirectionalLight& DirectionalLight::setDirection(const glm::vec3& direction) {
	_transform.setPosition(-glm::normalize(direction) * 10.0f);
	_shadowViewMatrix = glm::lookAt(_transform.getPosition(), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	_shadowViewProjection = _shadowProjectionMatrix * _shadowViewMatrix;
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
	const float aspect = static_cast<float>(_shadowBuffer->getWidth()) / static_cast<float>(_shadowBuffer->getHeight());
	setProjection(glm::perspective(60.0f, aspect, 0.1f, 100.0f));
	_transform.setPosition(position);
	viewMatrixCalc();
	_shadowBuffer->setName("Shadow Buffer (Point light)");
}

const glm::vec3& PointLight::getPosition() const { return _transform.getPosition(); }
PointLight& PointLight::setPosition(const glm::vec3& position) {
	_transform.setPosition(position);
	viewMatrixCalc();
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
	const float aspect = static_cast<float>(_shadowBuffer->getWidth()) / static_cast<float>(_shadowBuffer->getHeight());
	setProjection(glm::perspective(60.0f, aspect, 0.1f, 100.0f));
	_transform.setPosition(position).setRotation(direction);
	viewMatrixCalc();
	_shadowBuffer->setName("Shadow Buffer (Spotlight)");
}

const glm::vec3& Spotlight::getPosition() const { return _transform.getPosition(); }
Spotlight& Spotlight::setPosition(const glm::vec3& position) {
	_transform.setPosition(position);
	viewMatrixCalc();
	return *this;
}
Spotlight& Spotlight::setPosition(const float x, const float y, const float z) { return setPosition({ x, y, z }); }

const glm::vec3& Spotlight::getDirection() const { return _transform.getRotation(); }
Spotlight& Spotlight::setDirection(const glm::vec3& direction) {
	_transform.setRotation(direction);
	viewMatrixCalc();
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
