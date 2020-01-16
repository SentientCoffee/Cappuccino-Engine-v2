#include "CappPCH.h"
#include "Cappuccino/Objects/Hitbox.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

using namespace Capp;

// --------------------------------------------------------------------------------
// ----- Hitbox -------------------------------------------------------------------
// --------------------------------------------------------------------------------

bool Hitbox::_drawHitboxes = false;

Hitbox::~Hitbox() {
	delete _vao;
}

void Hitbox::setShouldDraw(const bool draw) { _drawHitboxes = draw; }
bool Hitbox::shouldDraw() { return _drawHitboxes; }

VertexArray* Hitbox::getVAO() const { return _vao; }

const glm::vec3& Hitbox::getPosition() const { return _transform.getPosition(); }
Transform& Hitbox::setPosition(const glm::vec3& position) { return _transform.setPosition(position); }
Transform& Hitbox::setPosition(const float x, const float y, const float z) { return _transform.setPosition(x, y, z); }

const glm::vec3& Hitbox::getRotation() const { return _transform.getRotation(); }
Transform& Hitbox::setRotation(const glm::vec3& eulerRotation) { return _transform.setRotation(eulerRotation); }
Transform& Hitbox::setRotation(const float x, const float y, const float z) { return _transform.setRotation(x, y, z); }

const Transform& Hitbox::getTransform() const { return _transform; }

// --------------------------------------------------------------------------------
// ----- Hitbox cube --------------------------------------------------------------
// --------------------------------------------------------------------------------

HitboxCube::HitboxCube(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec3& rotation) {
	_transform.setPosition(position).setScale(dimensions).setRotation(rotation);

	_vertices = {
		{ { -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f } },
		{ {  0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f } },
		{ { -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
		{ {  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f } },
		{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f } },
		{ {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f } },
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f } },
		{ {  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f } }
	};

	const std::vector<unsigned> indices = {
		0, 1, 3,	0, 2, 3,
		4, 5, 7,	4, 6, 7,
		4, 5, 1,	4, 0, 1,
		2, 6, 7,	2, 3, 7,
		0, 4, 6,	0, 2, 6,
		1, 5, 7,	1, 3, 7
	};
	
	_vao = new VertexArray;
	const auto vbo = new VertexBuffer(_vertices);
	const auto ibo = new IndexBuffer(indices);

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

bool HitboxCube::checkCollision(HitboxCube* other) const {
	// https://gamedev.stackexchange.com/questions/25397/obb-vs-obb-collision-detection

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

bool HitboxCube::checkCollision(HitboxSphere* other) const {
	const glm::vec3 spherePos = other->_transform.getPosition();
	const float sphereRadius = other->_transform.getScale().x;
	const glm::vec3 boxPos = _transform.getPosition();
	const glm::vec3 boxSize = _transform.getScale();
	
	float distance = 0.0f;
	distance += getDistance(spherePos.x, boxPos.x, boxSize.x);
	distance += getDistance(spherePos.y, boxPos.y, boxSize.y);
	distance += getDistance(spherePos.z, boxPos.z, boxSize.z);
	
	return distance <= sphereRadius * sphereRadius ;
}

float HitboxCube::getDistance(const float spherePos, const float boxPos, const float boxSize) const {
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

	_vao = new VertexArray;
	const auto vbo = new VertexBuffer(vertices);
	const auto ibo = new IndexBuffer(indices);

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

bool HitboxSphere::checkCollision(HitboxSphere* other) const {
	const float thisRadiusSq = _transform.getScale().x;// *_transform.getScale().x;
	const float otherRadiusSq = other->_transform.getScale().x;// *other->_transform.getScale().x;
	const float minDistance = thisRadiusSq + otherRadiusSq;

	const glm::vec3 differenceVector = _transform.getPosition() - other->_transform.getPosition();
	const float actualDistance = glm::length(differenceVector);

	return actualDistance < minDistance;
}

bool HitboxSphere::checkCollision(HitboxCube* other) {
	return other->checkCollision(this);
}

