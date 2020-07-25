#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Light.h"

#define DEFAULT_SHADOW_RESOLUTION 4096u

using namespace Capp;

// --------------------------------------------------------------------------
// ----- Light --------------------------------------------------------------
// --------------------------------------------------------------------------


Light::Light(const glm::vec3& colour) :
	_colour(colour)
{
	_shadowBuffer = Framebuffer::create(DEFAULT_SHADOW_RESOLUTION, DEFAULT_SHADOW_RESOLUTION);
}

void Light::viewMatrixCalc() {
	_shadowViewMatrix = glm::inverse(_transform.getLocalTransform());
	_shadowViewProjection = _shadowProjectionMatrix * _shadowViewMatrix;
}

void Light::setShadowResolution(const uint32_t resolution) const { _shadowBuffer->resize(resolution, resolution); }

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
	const Attachment depth = { AttachmentType::Texture, InternalFormat::Depth16,{ WrapMode::ClampToBorder, MinFilter::Nearest, MagFilter::Nearest, Anisotropy::Aniso1x, glm::vec4(1.0f) } };
	_shadowBuffer->addAttachment(AttachmentTarget::Depth, depth);
}

DirectionalLight& DirectionalLight::setDirection(const glm::vec3& direction) {
	_transform.setPosition(-glm::normalize(direction) * 20.0f);
	glm::vec3 upVec;
	
	if(glm::normalize(direction) == glm::vec3(0.0f, 1.0f, 0.0f) || glm::normalize(direction) == glm::vec3(0.0f, -1.0f, 0.0f)) {
		upVec = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	else {
		upVec = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	
	_shadowViewMatrix = glm::lookAt(_transform.getPosition(), glm::vec3(0.0f), upVec);
	_shadowViewProjection = _shadowProjectionMatrix * _shadowViewMatrix;
	_transform.setRotation(direction);
	return *this;
}

DirectionalLight& DirectionalLight::setColour(const glm::vec3& colour) {
	_colour = colour;
	return *this;
}

const glm::vec3& DirectionalLight::getDirection() const { return _transform.getRotation(); }

DirectionalLight& DirectionalLight::setDirection(const float x, const float y, const float z) { return setDirection({ x, y, z }); }
DirectionalLight& DirectionalLight::setColour(const float r, const float g, const float b) { return setColour({ r, g, b }); }

// --------------------------------------------------------------------------
// ----- Point light --------------------------------------------------------
// --------------------------------------------------------------------------

PointLight::PointLight(const glm::vec3& position, const glm::vec3& colour, const float attenuation) :
	Light(colour), _attenuation(attenuation)
{
	const float aspect = static_cast<float>(_shadowBuffer->getWidth()) / static_cast<float>(_shadowBuffer->getHeight());
	setProjection(glm::perspective(glm::radians(90.0f), aspect, 0.1f, 100.0f));
	_transform.setPosition(position);
	viewMatrixCalc();
	_shadowBuffer->setName("Shadow Buffer (Point light)");
	const Attachment depth = {
		AttachmentType::Cubemap,
		InternalFormat::Depth32F,
		{ WrapMode::ClampToBorder, MinFilter::Nearest, MagFilter::Nearest, Anisotropy::Aniso1x, glm::vec4(1.0f) }
	};
	_shadowBuffer->addAttachment(AttachmentTarget::Depth, depth);
}

PointLight& PointLight::setPosition(const glm::vec3& position) {
	_transform.setPosition(position);
	viewMatrixCalc();
	return *this;
}

PointLight& PointLight::setColour(const glm::vec3& colour) {
	_colour = colour;
	return *this;
}

PointLight& PointLight::setAttenuation(const float attenuation) {
	_attenuation = attenuation;
	return *this;
}

const glm::vec3& PointLight::getPosition() const { return _transform.getPosition(); }

PointLight& PointLight::setPosition(const float x, const float y, const float z) { return setPosition({ x, y, z }); }
PointLight& PointLight::setColour(const float r, const float g, const float b) { return setColour({ r, g, b }); }

// --------------------------------------------------------------------------
// ----- Spotlight ----------------------------------------------------------
// --------------------------------------------------------------------------

Spotlight::Spotlight(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& colour, const float attenuation, const float innerCutoffAngle, const float outerCutoffAngle) :
	Light(colour), _attenuation(attenuation), _innerCutoffAngle(innerCutoffAngle), _outerCutoffAngle(outerCutoffAngle)
{
	const float aspect = static_cast<float>(_shadowBuffer->getWidth()) / static_cast<float>(_shadowBuffer->getHeight());
	setProjection(glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f));
	_transform.setPosition(position).setRotation(direction);
	viewMatrixCalc();
	_shadowBuffer->setName("Shadow Buffer (Spotlight)");
	const Attachment depth = { AttachmentType::Texture, InternalFormat::Depth16,{ WrapMode::ClampToBorder, MinFilter::Nearest, MagFilter::Nearest, Anisotropy::Aniso1x, glm::vec4(1.0f) } };
	_shadowBuffer->addAttachment(AttachmentTarget::Depth, depth);
}

Spotlight& Spotlight::setPosition(const glm::vec3& position) {
	_transform.setPosition(position);
	viewMatrixCalc();
	return *this;
}

Spotlight& Spotlight::setDirection(const glm::vec3& direction) {
	_transform.setRotation(direction);
	viewMatrixCalc();
	return *this;
}

Spotlight& Spotlight::setColour(const glm::vec3& colour) {
	_colour = colour;
	return *this;
}

Spotlight& Spotlight::setAttenuation(const float attenuation) {
	_attenuation = attenuation;
	return *this;
}

Spotlight& Spotlight::setInnerCutoffAngle(const float angle) {
	_innerCutoffAngle = glm::min(angle, _outerCutoffAngle);
	return *this;
}

Spotlight& Spotlight::setOuterCutoffAngle(const float angle) {
	_outerCutoffAngle = glm::max(angle, _innerCutoffAngle);
	return *this;
}

const glm::vec3& Spotlight::getPosition() const { return _transform.getPosition(); }
const glm::vec3& Spotlight::getDirection() const { return _transform.getRotation(); }

Spotlight& Spotlight::setPosition(const float x, const float y, const float z) { return setPosition({ x, y, z }); }
Spotlight& Spotlight::setDirection(const float x, const float y, const float z) { return setDirection({ x, y, z }); }
Spotlight& Spotlight::setColour(const float r, const float g,const float b) { return setColour({ r, g, b }); }