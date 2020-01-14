#include "CappPCH.h"
#include "Cappuccino/Rendering/RenderCommand.h"

#include "Cappuccino/Rendering/3D/Renderer.h"
#include "Cappuccino/Rendering/2D/Renderer2D.h"

#include <glad/glad.h>

using namespace Capp;

void RenderCommand::init() {
	// Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Depth testing
	glEnable(GL_DEPTH_TEST);

	// Scissor testing (for multiple viewports)
	glEnable(GL_SCISSOR_TEST);

	// Initialize 2D and 3D renderers
	Renderer::init();
	Renderer2D::init();
}

void RenderCommand::shutdown() {
	Renderer::shutdown();
	Renderer2D::shutdown();
}

void RenderCommand::setClearColour(const glm::vec4& colour) {
	glClearColor(colour.r, colour.g, colour.b, colour.a);
}

void RenderCommand::setClearColour(const float r, const float g, const float b, const float a) {
	glClearColor(r, g, b, a);
}

void RenderCommand::clearScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderCommand::enableCulling() {
	glEnable(GL_CULL_FACE);
}

void RenderCommand::disableCulling() {
	glDisable(GL_CULL_FACE);
}

void RenderCommand::drawArray(VertexArray* vertexArray) {
	unsigned count = 0;
	for(auto vbo : vertexArray->getVertexBuffers()) {
		count += vbo->getCount();
	}
	
	glDrawArrays(GL_TRIANGLES, 0, count);
}

void RenderCommand::drawIndexed(VertexArray* vertexArray) {
	glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}
