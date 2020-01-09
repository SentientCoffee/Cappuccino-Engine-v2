#include "CappPCH.h"
#include "Cappuccino/Rendering/Renderer.h"

#include "Cappuccino/Rendering/RenderCommand.h"

using namespace Capp;

void Renderer::beginScene() {}
void Renderer::endScene() {}

void Renderer::addToRenderList(VertexArray* vertexArray) {
	vertexArray->bind();
	if(vertexArray->getIndexBuffer()->getCount() > 0) {
		RenderCommand::drawIndexed(vertexArray);
	}
	else {
		RenderCommand::drawArray(vertexArray);
	}
}
