#include "Objects/F16.h"

F16::F16() {
	unsigned blackTexture = 0x00000000;
	
	auto material = new Capp::Material();
	material->setValue("diffuse", new Capp::Texture2D("Assets/Textures/Checkerboard.png"));
	material->setValue("specular", new Capp::Texture2D("Assets/Textures/CheckerboardSpecular.png"));
	material->setValue("emission", new Capp::Texture2D(1, 1, &blackTexture));
	material->setValue("roughness", 0.22f);

	Capp::Mesh* mesh = Capp::MeshLibrary::loadMesh("F16", "Assets/Meshes/F16.obj");

	_models = { new Capp::Model(mesh, material) };
}

F16::~F16() = default;

void F16::update() {
	rotateBy(0.0f, 45.0f, 0.0f);
}

void F16::onEvent(Capp::Event& e) {}
