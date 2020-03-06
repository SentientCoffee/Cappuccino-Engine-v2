#include "Objects/Captain.h"

Captain::Captain() {
	auto material = new Capp::Material();
	material->setValue("diffuse", new Capp::Texture2D("Assets/Textures/Captain/Captain_BaseColor.png"));
	material->setValue("specular", new Capp::Texture2D("Assets/Textures/Captain/Captain_Metallic.png"));
	material->setValue("emission", new Capp::Texture2D("Assets/Textures/Captain/Captain_Emission.png"));
	material->setValue("roughness", 0.22f);

	Capp::Mesh* mesh = Capp::MeshLibrary::loadMesh("Captain", "Assets/Meshes/Captain.obj");

	_models = { new Capp::Model(mesh, material) };
}

Captain::~Captain() = default;

void Captain::update() {
	rotateBy(0.0f, -45.0f, 0.0f);
}

void Captain::onEvent(Capp::Event& e) {
	
}