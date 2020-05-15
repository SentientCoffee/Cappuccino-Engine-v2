#include "CappPCH.h"
#include "Cappuccino/Objects/Hitbox.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/norm.hpp>

using namespace Capp;

// --------------------------------------------------------------------------------
// ----- Hitbox -------------------------------------------------------------------
// --------------------------------------------------------------------------------

bool Hitbox::_drawHitboxes = false;

Hitbox& Hitbox::setPosition(const glm::vec3& position) {
	_transform.setPosition(position);
	return *this;
}

Hitbox& Hitbox::setRotation(const glm::vec3& degrees) {
	_transform.setRotation(degrees);
	return *this;
}

// --------------------------------------------------------------------------------
// ----- Hitbox cube --------------------------------------------------------------
// --------------------------------------------------------------------------------

HitboxCube::HitboxCube(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec3& rotation) {
	_transform.setPosition(position).setScale(dimensions).setRotation(rotation);

	_vertices = {
		{ { -0.5f,  0.5f,  0.5f } },
		{ {  0.5f,  0.5f,  0.5f } },
		{ { -0.5f, -0.5f,  0.5f } },
		{ {  0.5f, -0.5f,  0.5f } },
		{ { -0.5f,  0.5f, -0.5f } },
		{ {  0.5f,  0.5f, -0.5f } },
		{ { -0.5f, -0.5f, -0.5f } },
		{ {  0.5f, -0.5f, -0.5f } }
	};

	const std::vector<unsigned> indices = {
		0, 1, 3,	0, 2, 3,
		4, 5, 7,	4, 6, 7,
		4, 5, 1,	4, 0, 1,
		2, 6, 7,	2, 3, 7,
		0, 4, 6,	0, 2, 6,
		1, 5, 7,	1, 3, 7
	};
	
	_vao = VertexArray::create();
	const auto vbo = VertexBuffer::create(_vertices);
	const auto ibo = IndexBuffer::create(indices);

	const BufferLayout layout = {
		{ ShaderDataType::Vec3, "inPosition" },
		{ ShaderDataType::Vec2, "inUV" },
		{ ShaderDataType::Vec3, "inNormal" }
	};

	vbo->setLayout(layout);
	_vao->addVertexBuffer(vbo);
	_vao->setIndexBuffer(ibo);
}

const glm::vec3& HitboxCube::getScale() const { return _transform.getScale(); }
Transform& HitboxCube::setScale(const glm::vec3& scale) { return _transform.setScale(scale); }
Transform& HitboxCube::setScale(const float x, const float y, const float z) { return _transform.setScale(x, y, z); }
Transform& HitboxCube::setScale(const float scale) { return _transform.setScale(scale); }

bool HitboxCube::checkCollision(const Ref<HitboxCube>& other) const {
	// https://gamedev.stackexchange.com/questions/25397/obb-vs-obb-collision-detection (for SAT algorithm)
	// https://www.babylonjs-playground.com/#235QZM#5 (for getting normals)

	std::vector<glm::vec3> normalsThis = {
		{  _transform.getWorldTransform()[0] },
		{ -_transform.getWorldTransform()[0] },
		{  _transform.getWorldTransform()[1] },
		{ -_transform.getWorldTransform()[1] },
		{  _transform.getWorldTransform()[2] },
		{ -_transform.getWorldTransform()[2] }
	};
	
	for(auto normal : normalsThis) {
		auto [box1Min, box1Max] = getSATMinMax(normal);
		auto [box2Min, box2Max] = other->getSATMinMax(normal);
		if(!(box1Min <= box2Min && box2Min <= box1Max || box2Min <= box1Min && box1Min <= box2Max)) {
			// If there is no overlap, return false
			return false;
		}
	}

	std::vector<glm::vec3> normalsOther = {
		{  other->_transform.getWorldTransform()[0] },
		{ -other->_transform.getWorldTransform()[0] },
		{  other->_transform.getWorldTransform()[1] },
		{ -other->_transform.getWorldTransform()[1] },
		{  other->_transform.getWorldTransform()[2] },
		{ -other->_transform.getWorldTransform()[2] }
	};

	for(auto normal : normalsOther) {
		auto [box1Min, box1Max] = getSATMinMax(normal);
		auto [box2Min, box2Max] = other->getSATMinMax(normal);
		if(!(box1Min <= box2Min && box2Min <= box1Max || box2Min <= box1Min && box1Min <= box2Max)) {
			// If there is no overlap, return false
			return false;
		}
	}
	
	return true;
}

bool HitboxCube::checkCollision(const Ref<HitboxSphere>& other) const {
	const glm::vec3 spherePos = other->_transform.getPosition();
	const float sphereRadius = other->_transform.getScale().x;
	const glm::vec3 boxPos = _transform.getPosition();
	const glm::vec3 boxSize = _transform.getScale();
	
	float distance = 0.0f;
	distance += getDistance(spherePos.x, boxPos.x, boxSize.x);
	distance += getDistance(spherePos.y, boxPos.y, boxSize.y);
	distance += getDistance(spherePos.z, boxPos.z, boxSize.z);
	
	return distance <= sphereRadius * sphereRadius;
}

float HitboxCube::getDistance(const float spherePos, const float boxPos, const float boxSize) {
	float distance = 0.0f;
	if(spherePos < boxPos - boxSize / 2.0f) {
		const float dist = boxPos - boxSize / 2.0f - spherePos;
		distance += dist * dist;
	}
	if(spherePos > boxSize / 2.0f) {
		const float dist = spherePos - boxSize / 2.0f;
		distance += dist * dist;
	}

	return distance;
}


std::tuple<float, float> HitboxCube::getSATMinMax(const glm::vec3& axis) const {
	float minimum = 10000000.0f;
	float maximum = -10000000.0f;
	
	for(auto vertex : _vertices) {
		glm::vec4 transformedVertex = _transform.getWorldTransform() * glm::vec4(vertex.position, 1.0f);
		
		const float projected = glm::dot(glm::vec3(transformedVertex), axis);
		if(projected < minimum) minimum = projected;
		if(projected > maximum) maximum = projected;
	}

	return { minimum, maximum };
}

// --------------------------------------------------------------------------------
// ----- Hitbox sphere ------------------------------------------------------------
// --------------------------------------------------------------------------------

HitboxSphere::HitboxSphere(const glm::vec3& position, const float radius) {
	_transform.setPosition(position).setScale(radius);
	
	const std::vector<Vertex> vertices = {
		{ {  0.0f,  0.5f,  0.0f }, { 0.0f, 0.0f } },
		{ {  0.0f,  0.0f, -0.5f }, { 0.0f, 0.0f } },
		{ { -0.5f,  0.0f,  0.0f }, { 0.0f, 0.0f } },
		{ {  0.0f,  0.0f,  0.5f }, { 0.0f, 0.0f } },
		{ {  0.5f,  0.0f,  0.0f }, { 0.0f, 0.0f } },
		{ {  0.0f, -0.5f,  0.0f }, { 0.0f, 0.0f } }
	};

	const std::vector<unsigned> indices = {
		0, 3, 4,
		0, 1, 4,
		0, 1, 2,
		0, 2, 3,
		5, 3, 4,
		5, 1, 4,
		5, 1, 2,
		5, 2, 3
	};

	_vao = VertexArray::create();
	const auto vbo = VertexBuffer::create(vertices);
	const auto ibo = IndexBuffer::create(indices);

	const BufferLayout layout = {
		{ ShaderDataType::Vec3, "inPosition" },
		{ ShaderDataType::Vec2, "inUV" },
		{ ShaderDataType::Vec3, "inNormal" }
	};

	vbo->setLayout(layout);
	_vao->addVertexBuffer(vbo);
	_vao->setIndexBuffer(ibo);
}

float HitboxSphere::getScale() const { return _transform.getScale().x; }
Transform& HitboxSphere::setScale(const float scale) { return _transform.setScale(scale); }

bool HitboxSphere::checkCollision(const Ref<HitboxSphere>& other) const {
	const float thisRadiusSq = _transform.getScale().x;// *_transform.getScale().x;
	const float otherRadiusSq = other->_transform.getScale().x;// *other->_transform.getScale().x;
	const float minDistance = thisRadiusSq + otherRadiusSq;

	const glm::vec3 differenceVector = _transform.getPosition() - other->_transform.getPosition();
	const float actualDistance = glm::length/*2*/(differenceVector);

	return actualDistance < minDistance;
}

bool HitboxSphere::checkCollision(const Ref<HitboxCube>& other) const {
	const glm::vec3 spherePos = _transform.getPosition();
	const float sphereRadius = _transform.getScale().x;
	const glm::vec3 boxPos = other->_transform.getPosition();
	const glm::vec3 boxSize = other->_transform.getScale();

	float distance = 0.0f;
	distance += other->getDistance(spherePos.x, boxPos.x, boxSize.x);
	distance += other->getDistance(spherePos.y, boxPos.y, boxSize.y);
	distance += other->getDistance(spherePos.z, boxPos.z, boxSize.z);

	return distance <= sphereRadius * sphereRadius;
}

