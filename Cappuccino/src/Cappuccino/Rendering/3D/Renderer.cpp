#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Renderer.h"

#include "Cappuccino/Core/Application.h"
#include "Cappuccino/Rendering/RenderCommand.h"
#include "Cappuccino/Rendering/2D/Renderer2D.h"

using namespace Capp;

struct RendererStorage {
	glm::mat4 viewProjection = glm::mat4(1.0f);
};

static RendererStorage* rendererStorage;

void Renderer::init() {
	rendererStorage = new RendererStorage;
	RenderCommand::setClearColour(0.1f, 0.1f, 0.1f, 1.0f);
}

void Renderer::shutdown() {
	delete rendererStorage;
}

void Renderer::start() {
	const auto window = Application::getInstance()->getWindow();
	PerspectiveCamera defaultCam(60.0f, window->getWidth(), window->getHeight());
	defaultCam.setPosition(0.0f, 5.0f, 5.0f);
	defaultCam.lookAt(0.0f, 0.0f, 0.0f);
	start(defaultCam);
}

void Renderer::start(const PerspectiveCamera& camera) {
	rendererStorage->viewProjection = camera.getViewProjection();
}

void Renderer::finish() {
	Shader::unbind();
	Texture2D::unbind();
	VertexArray::unbind();
}

void Renderer::addToRenderList(Shader* shader, VertexArray* vertexArray) {
	shader->bind();
	shader->setUniform("uViewProjection", rendererStorage->viewProjection);
	
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
