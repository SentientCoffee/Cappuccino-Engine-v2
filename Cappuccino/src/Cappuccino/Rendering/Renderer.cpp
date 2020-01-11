#include "CappPCH.h"
#include "Cappuccino/Rendering/Renderer.h"

#include "Cappuccino/Rendering/RenderCommand.h"

using namespace Capp;

Renderer::RenderData* Renderer::_renderData = new RenderData;

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
