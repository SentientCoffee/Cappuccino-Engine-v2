#include "Objects/SentryBot.h"

SentryBot::SentryBot() {
	auto material = new Capp::Material();
	material->setValue("diffuse", new Capp::Texture2D("Assets/Textures/Sentry/Sentry-Diffuse.png"));
	material->setValue("specular", new Capp::Texture2D("Assets/Textures/Sentry/Sentry-Metallic.png"));
	material->setValue("emission", new Capp::Texture2D("Assets/Textures/Sentry/Sentry-Emission.png"));
	material->setValue("roughness", 0.22f);

	Capp::Mesh* mesh = Capp::MeshLibrary::loadMesh("Sentry", "Assets/Meshes/Sentry.obj");

	_models = { new Capp::Model(mesh, material) };
}

SentryBot::~SentryBot() = default;

void SentryBot::update() {
	if(_rigidbody.getScale().x > 2.0f) {
		scaleUp = false;
	}
	if(_rigidbody.getScale().x < 0.5f) {
		scaleUp = true;
	}

	if(scaleUp) {
		scaleBy(0.01f);
	}
	else {
		scaleBy(-0.01f);
	}
}

void SentryBot::onEvent(Capp::Event& e) {}
