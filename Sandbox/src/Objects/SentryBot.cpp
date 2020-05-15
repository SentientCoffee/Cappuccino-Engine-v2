#include "Objects/SentryBot.h"

SentryBot::SentryBot() {
	auto material = Capp::Material::create();
	material->setDiffuseMap(Capp::Texture2D::create("Assets/Textures/Sentry/Sentry-Diffuse.png"));
	material->setSpecularMap(Capp::Texture2D::create("Assets/Textures/Sentry/Sentry-Metallic.png"));
	material->setEmissionMap(Capp::Texture2D::create("Assets/Textures/Sentry/Sentry-Emission.png"));
	material->setRoughness(0.22f);

	const Ref<Capp::Mesh> mesh = Capp::MeshLibrary::loadMesh("Sentry", "Assets/Meshes/Sentry.obj");

	_models = { Capp::Model::create(mesh, material) };
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
