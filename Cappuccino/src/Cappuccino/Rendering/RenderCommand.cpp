#include "CappPCH.h"
#include "Cappuccino/Rendering/RenderCommand.h"

#include <glad/glad.h>

using namespace Capp;

void RenderCommand::init() {
	// Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderCommand::setClearColour(const glm::vec4& colour) { glClearColor(colour.r, colour.g, colour.b, colour.a); }
void RenderCommand::setClearColour(const float r, const float g, const float b, const float a) { glClearColor(r, g, b, a); }
void RenderCommand::clearScreen() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void RenderCommand::drawArray(VertexArray* vertexArray) {
	unsigned count = 0;
	for(auto vbo : vertexArray->getVertexBuffers()) {
		count += vbo->getCount();
	}
	
	glDrawArrays(GL_TRIANGLES, 0, count);
}

void RenderCommand::drawIndexed(VertexArray* vertexArray) { glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr); }
