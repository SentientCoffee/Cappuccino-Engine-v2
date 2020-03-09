#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Model.h"

using namespace Capp;

Model::Model(Mesh* mesh, Material* material) :
	_mesh(mesh), _material(material) {}

Model::~Model() {
	delete _material;
}

Mesh* Model::getMesh() const { return _mesh; }
void Model::setMesh(Mesh* mesh) { _mesh = mesh; }

Material* Model::getMaterial() const { return _material; }
void Model::setMaterial(Material* material) { _material = material; }

const glm::vec3& Model::getPosition() const { return _transform.getPosition(); }
Model& Model::setPosition(const glm::vec3& position) {
	_transform.setPosition(position);
	return *this;
}
Model& Model::setPosition(const float x, const float y, const float z) { return setPosition({ x, y, z }); }

const glm::vec3& Model::getRotation() const { return _transform.getRotation(); }
Model& Model::setRotation(const glm::vec3& eulerRotation) {
	_transform.setRotation(eulerRotation);
	return *this;
}
Model& Model::setRotation(const float x, const float y, const float z) { return setRotation({ x, y, z }); }

const glm::vec3& Model::getScale() const { return _transform.getScale(); }
Model& Model::setScale(const glm::vec3& scale) {
	_transform.setScale(scale);
	return *this;
}
Model& Model::setScale(const float x, const float y, const float z) { return setScale({ x, y, z }); }
Model& Model::setScale(const float scale) { return setScale({ scale, scale, scale }); }

const Transform& Model::getTransform() const { return _transform; }
