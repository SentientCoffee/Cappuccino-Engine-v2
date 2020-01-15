#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Model.h"

using namespace Capp;

Model::Model(Mesh* mesh, Material* material) :
	_mesh(mesh), _material(material) {}

Model::~Model() {
	delete _mesh;
	delete _material;
}

Mesh* Model::getMesh() const { return _mesh; }
void Model::setMesh(Mesh* mesh) { _mesh = mesh; }

Material* Model::getMaterial() const { return _material; }
void Model::setMaterial(Material* material) { _material = material; }

const glm::vec3& Model::getPosition() const { return _transform.getPosition(); }
Transform& Model::setPosition(const glm::vec3& position) { return _transform.setPosition(position); }
Transform& Model::setPosition(const float x, const float y, const float z) { return _transform.setPosition(x, y, z); }

const glm::vec3& Model::getRotation() const { return _transform.getRotation(); }
Transform& Model::setRotation(const glm::vec3& eulerRotation) { return _transform.setRotation(eulerRotation); }
Transform& Model::setRotation(const float x, const float y, const float z) { return _transform.setRotation(x, y, z); }

const glm::vec3& Model::getScale() const { return _transform.getScale(); }
Transform& Model::setScale(const glm::vec3& scale) { return _transform.setScale(scale); }
Transform& Model::setScale(const float x, const float y, const float z) { return _transform.setScale(x, y, z); }
Transform& Model::setScale(const float scale) { return _transform.setScale(scale); }

const Transform& Model::getTransform() const { return _transform; }
