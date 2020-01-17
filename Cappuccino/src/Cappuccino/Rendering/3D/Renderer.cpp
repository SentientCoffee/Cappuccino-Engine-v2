#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Renderer.h"

#include "Cappuccino/Core/Application.h"
#include "Cappuccino/Rendering/RenderCommand.h"
#include "Cappuccino/Rendering/2D/Renderer2D.h"

using namespace Capp;

struct RendererStorage {
	PerspectiveCamera perspectiveCamera;
	Shader* hitboxShader = nullptr;
};

static RendererStorage* rendererStorage;

void Renderer::init() {
	rendererStorage = new RendererStorage;
	rendererStorage->hitboxShader = new Shader("DefaultHitbox", "Assets/Cappuccino/Shaders/HitboxShader.vert", "Assets/Cappuccino/Shaders/HitboxShader.frag");

	const auto window = Application::getInstance()->getWindow();
	rendererStorage->perspectiveCamera.setProjection(60.0f, window->getWidth(), window->getHeight());
	rendererStorage->perspectiveCamera.setPosition(0.0f, 5.0f, 5.0f);
	rendererStorage->perspectiveCamera.lookAt(0.0f, 0.0f, 0.0f);
	
	RenderCommand::setClearColour(0.1f, 0.1f, 0.1f, 1.0f);
}

void Renderer::shutdown() {
	delete rendererStorage;
}

void Renderer::onWindowResized(const unsigned width, const unsigned height) {
	rendererStorage->perspectiveCamera.setProjection(60.0f, width, height);
}

void Renderer::start() {
	RenderCommand::enableCulling();
}

void Renderer::start(const PerspectiveCamera& camera) {
	RenderCommand::enableCulling();
	rendererStorage->perspectiveCamera = camera;
}

void Renderer::finish() {
	Shader::unbind();
	Texture2D::unbind();
	VertexArray::unbind();
}

void Renderer::addToRenderList(Shader* shader, VertexArray* vertexArray) {
	shader->bind();
	shader->setUniform("uViewProjection", rendererStorage->perspectiveCamera.getViewProjection());
	
	vertexArray->bind();
	if(vertexArray->getIndexBuffer() != nullptr) {
		RenderCommand::drawIndexed(vertexArray);
	}
	else {
		RenderCommand::drawArray(vertexArray);
	}
}

void Renderer::addToRenderList(Material* material, VertexArray* vertexArray) {
	material->apply();
	addToRenderList(material->getShader(), vertexArray);
}

void Renderer::addToRenderList(Shader* shader, Mesh* mesh) {
	addToRenderList(shader, mesh->getVAO());
}

void Renderer::addToRenderList(Material* material, Mesh* mesh) {
	material->apply();
	addToRenderList(material->getShader(), mesh);
}

void Renderer::addToRenderList(Model* model) {
	model->getMaterial()->setValue("uTransform", model->getTransform().getWorldTransform());
	addToRenderList(model->getMaterial(), model->getMesh());
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
	addToRenderList(rendererStorage->hitboxShader, hitbox->getVAO());

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

	addToRenderList(gameObject->getRigidBody());
}
