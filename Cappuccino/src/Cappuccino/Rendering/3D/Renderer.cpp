#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Renderer.h"

#include "Cappuccino/Core/Application.h"
#include "Cappuccino/Rendering/RenderCommand.h"
#include "Cappuccino/Rendering/2D/Renderer2D.h"

using namespace Capp;

struct RendererStorage {
	PerspectiveCamera defaultCamera;
	PerspectiveCamera perspectiveCamera;
	
	Shader* defaultShader = nullptr;
	Shader* activeShader = nullptr;
	
	Shader* hitboxShader = nullptr;

	std::vector<PointLight*> defaultPointLights;
	std::vector<PointLight*> pointLights;
	
	std::vector<DirectionalLight*> directionalLights;
	std::vector<DirectionalLight*> defaultDirectionalLights;

	std::vector<Spotlight*> spotlights;
	std::vector<Spotlight*> defaultSpotlights;
};

static RendererStorage* rendererStorage;

void Renderer::init() {
	rendererStorage = new RendererStorage;

	const auto window = Application::getInstance()->getWindow();
	rendererStorage->defaultCamera.setProjection(60.0f, window->getWidth(), window->getHeight());
	rendererStorage->defaultCamera.setPosition(0.0f, 5.0f, 5.0f);
	rendererStorage->defaultCamera.lookAt(0.0f, 0.0f, 0.0f);

	rendererStorage->defaultShader = new Shader("Default3D", "Assets/Cappuccino/Shaders/BlinnPhongShader.vert", "Assets/Cappuccino/Shaders/BlinnPhongShader.frag");
	rendererStorage->hitboxShader = new Shader("DefaultHitbox", "Assets/Cappuccino/Shaders/HitboxShader.vert", "Assets/Cappuccino/Shaders/HitboxShader.frag");

	const auto pointLight = new PointLight;
	pointLight->setPosition(0.0f, 2.0f, 5.0f);
	rendererStorage->defaultPointLights = { pointLight };

	//const auto dirLight = new DirectionalLight;
	//dirLight->setDirection(0.0f, 0.0f, 0.0f);
	rendererStorage->defaultDirectionalLights = { /*dirLight*/ };

	//const auto spotlight = new Spotlight;
	//spotlight->setPosition(rendererStorage->defaultCamera.getPosition());
	rendererStorage->defaultSpotlights = { /*spotlight*/ };
	
	
	RenderCommand::setClearColour(0.1f, 0.1f, 0.1f, 1.0f);
}

void Renderer::shutdown() {
	delete rendererStorage;
}

void Renderer::onWindowResized(const unsigned width, const unsigned height) {
	rendererStorage->defaultCamera.setProjection(60.0f, width, height);
}

void Renderer::start() {
	start(rendererStorage->defaultCamera, rendererStorage->defaultPointLights, rendererStorage->defaultDirectionalLights, rendererStorage->defaultSpotlights);
}

void Renderer::start(const PerspectiveCamera& camera, const std::vector<PointLight*>& pointLights, const std::vector<DirectionalLight*>& dirLights, const std::vector<Spotlight*>& spotlights) {
	rendererStorage->perspectiveCamera = camera;
	rendererStorage->directionalLights = dirLights;
	rendererStorage->pointLights = pointLights;
	rendererStorage->spotlights = spotlights;

	RenderCommand::enableCulling();
}

void Renderer::finish() {
	Shader::unbind();
	Texture2D::unbind();
	VertexArray::unbind();
}

void Renderer::addToRenderList(VertexArray* vertexArray, Shader* shader) {
	if(shader != nullptr) {
		rendererStorage->activeShader = shader;
	}
	else {
		rendererStorage->activeShader = rendererStorage->defaultShader;
	}
	
	rendererStorage->activeShader->bind();
	rendererStorage->activeShader->setUniform("uViewProjection", rendererStorage->perspectiveCamera.getViewProjection());
	rendererStorage->activeShader->setUniform("uCameraPosition", rendererStorage->perspectiveCamera.getPosition());
	rendererStorage->activeShader->setUniform("uNumPointLights", static_cast<int>(rendererStorage->pointLights.size()));
	rendererStorage->activeShader->setUniform("uNumDirectionalLights", static_cast<int>(rendererStorage->directionalLights.size()));
	rendererStorage->activeShader->setUniform("uNumSpotlights", static_cast<int>(rendererStorage->spotlights.size()));

	rendererStorage->activeShader->setUniform("uAmbientColour", { 1.0f, 1.0f, 1.0f });
	rendererStorage->activeShader->setUniform("uAmbientPower", 0.3f);

	for(unsigned i = 0; i < rendererStorage->directionalLights.size(); ++i) {
		rendererStorage->activeShader->setUniform("uDirectionalLights[" + std::to_string(i) + "].direction", rendererStorage->directionalLights[i]->getDirection());
		rendererStorage->activeShader->setUniform("uDirectionalLights[" + std::to_string(i) + "].colour", rendererStorage->directionalLights[i]->getColour());
	}

	for(unsigned i = 0; i < rendererStorage->pointLights.size(); ++i) {
		rendererStorage->activeShader->setUniform("uPointLights[" + std::to_string(i) + "].position", rendererStorage->pointLights[i]->getPosition());
		rendererStorage->activeShader->setUniform("uPointLights[" + std::to_string(i) + "].colour", rendererStorage->pointLights[i]->getColour());
		rendererStorage->activeShader->setUniform("uPointLights[" + std::to_string(i) + "].attenuation", rendererStorage->pointLights[i]->getAttenuation());
	}	

	for(unsigned i = 0; i < rendererStorage->spotlights.size(); ++i) {
		rendererStorage->activeShader->setUniform("uSpotlights[" + std::to_string(i) + "].position", rendererStorage->spotlights[i]->getPosition());
		rendererStorage->activeShader->setUniform("uSpotlights[" + std::to_string(i) + "].direction", rendererStorage->spotlights[i]->getDirection());
		rendererStorage->activeShader->setUniform("uSpotlights[" + std::to_string(i) + "].colour", rendererStorage->spotlights[i]->getColour());
		rendererStorage->activeShader->setUniform("uSpotlights[" + std::to_string(i) + "].attenuation", rendererStorage->spotlights[i]->getAttenuation());
		rendererStorage->activeShader->setUniform("uSpotlights[" + std::to_string(i) + "].innerCutoffAngle", rendererStorage->spotlights[i]->getInnerCutoffAngle());
		rendererStorage->activeShader->setUniform("uSpotlights[" + std::to_string(i) + "].outerCutoffAngle", rendererStorage->spotlights[i]->getOuterCutoffAngle());
	}
	
	vertexArray->bind();
	if(vertexArray->getIndexBuffer() != nullptr) {
		RenderCommand::drawIndexed(vertexArray);
	}
	else {
		RenderCommand::drawArray(vertexArray);
	}
}

void Renderer::addToRenderList(VertexArray* vertexArray, Material* material) {
	material->apply();
	addToRenderList(vertexArray, material->getShader());
}

void Renderer::addToRenderList(Mesh* mesh, Shader* shader) {
	addToRenderList(mesh->getVAO(), shader);
}

void Renderer::addToRenderList(Mesh* mesh, Material* material) {
	material->apply();
	addToRenderList(mesh, material->getShader());
}

void Renderer::addToRenderList(Model* model) {
	model->getMaterial()->getShader()->bind();
	model->getMaterial()->getShader()->setUniform("uTransform", model->getTransform().getWorldTransform());
	model->getMaterial()->getShader()->setUniform("uNormalMatrix", glm::mat3(glm::transpose(glm::inverse(model->getTransform().getWorldTransform()))));
	addToRenderList(model->getMesh(), model->getMaterial());
}

void Renderer::addToRenderList(Hitbox* hitbox) {
	if(!Hitbox::shouldDraw()) {
		return;
	}

	RenderCommand::disableCulling();
	RenderCommand::setDrawMode(DrawMode::Line);

	rendererStorage->hitboxShader->bind();
	rendererStorage->hitboxShader->setUniform("uHitboxColour", { 1.0f, 0.0f, 0.0f, 1.0f });
	rendererStorage->hitboxShader->setUniform("uTransform", hitbox->getTransform().getWorldTransform());
	rendererStorage->hitboxShader->setUniform("uViewProjection", rendererStorage->perspectiveCamera.getViewProjection());

	hitbox->getVAO()->bind();
	RenderCommand::drawIndexed(hitbox->getVAO());

	RenderCommand::setDrawMode(DrawMode::Fill);
	RenderCommand::enableCulling();
}

void Renderer::addToRenderList(const RigidBody& rigidBody) {
	if(!Hitbox::shouldDraw()) {
		return;
	}
	
	for(auto hitbox : rigidBody) {
		addToRenderList(hitbox);
	}
}

void Renderer::addToRenderList(GameObject* gameObject) {
	for(auto model : gameObject->getModels()) {
		addToRenderList(model);
	}

	if(gameObject->isVisible()) {
		addToRenderList(gameObject->getRigidBody());
	}
}
