#include "Objects/Captain.h"

Captain::Captain() {
	auto material = Capp::Material::create();
	material->setDiffuseMap(Capp::Texture2D::create("Assets/Textures/Captain/Captain_BaseColor.png"));
	material->setSpecularMap(Capp::Texture2D::create("Assets/Textures/Captain/Captain_Metallic.png"));
	material->setEmissionMap(Capp::Texture2D::create("Assets/Textures/Captain/Captain_Emission.png"));
	material->setRoughness(0.22f);

	const Ref<Capp::Mesh> mesh = Capp::MeshLibrary::loadMesh("Captain", "Assets/Meshes/Captain.obj");

	_models = { Capp::Model::create(mesh, material) };
}

Captain::~Captain() = default;

void Captain::update() {
	rotateBy(0.0f, -45.0f, 0.0f);
}

void Captain::onEvent(Capp::Event& e) {
	
}