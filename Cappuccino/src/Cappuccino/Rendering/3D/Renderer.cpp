#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Renderer.h"

#include "Cappuccino/Rendering/RenderCommand.h"
#include "Cappuccino/Rendering/2D/Renderer2D.h"

#include <glm/glm.hpp>

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

void Renderer::start() {}
void Renderer::finish() {}

void Renderer::addToRenderList(VertexArray* vertexArray) {
	vertexArray->bind();
	if(vertexArray->getIndexBuffer() != nullptr) {
		RenderCommand::drawIndexed(vertexArray);
	}
	else {
		RenderCommand::drawArray(vertexArray);
	}
}

void Renderer::addToRenderList(Mesh* mesh) {
	addToRenderList(mesh->getVAO());
}
