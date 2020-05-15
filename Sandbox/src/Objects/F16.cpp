#include "Objects/F16.h"

F16::F16() {
	unsigned blackTexture = 0x00000000;
	
	auto material = Capp::Material::create();
	material->setDiffuseMap(Capp::Texture2D::create("Assets/Textures/Checkerboard.png"));
	material->setSpecularMap(Capp::Texture2D::create("Assets/Textures/CheckerboardSpecular.png"));
	material->setEmissionMap(Capp::Texture2D::create(1, 1, &blackTexture));
	material->setRoughness(0.22f);

	const Ref<Capp::Mesh> mesh = Capp::MeshLibrary::loadMesh("F16", "Assets/Meshes/F16.obj");

	_models = { Capp::Model::create(mesh, material) };
}

F16::~F16() = default;

void F16::update() {
	rotateBy(0.0f, 45.0f, 0.0f);
}

void F16::onEvent(Capp::Event& e) {}
