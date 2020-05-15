#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Model.h"

using namespace Capp;

Model::Model(const Ref<Mesh>& mesh, const Ref<Material>& material) :
	_mesh(mesh), _material(material) {}

Model& Model::setPosition(const glm::vec3& position) {
	_transform.setPosition(position);
	return *this;
}

Model& Model::setRotation(const glm::vec3& degrees) {
	_transform.setRotation(degrees);
	return *this;
}

Model& Model::setScale(const glm::vec3& scale) {
	_transform.setScale(scale);
	return *this;
}

const glm::vec3& Model::getPosition() const { return _transform.getPosition(); }
const glm::vec3& Model::getRotation() const { return _transform.getRotation(); }
const glm::vec3& Model::getScale() const    { return _transform.getScale();    }

Model& Model::setPosition(const float x, const float y, const float z) { return setPosition({ x, y, z }); }
Model& Model::setRotation(const float x, const float y, const float z) { return setRotation({ x, y, z }); }
Model& Model::setScale(const float x, const float y, const float z)    { return setScale({ x, y, z }); }
Model& Model::setScale(const float scale)                              { return setScale({ scale, scale, scale }); }
