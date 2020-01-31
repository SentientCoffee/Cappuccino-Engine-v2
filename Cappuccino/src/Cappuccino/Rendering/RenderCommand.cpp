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

	// Scissor testing (for multiple viewports)
	glEnable(GL_SCISSOR_TEST);

	// Seamless cubemaps
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	// Initialize 2D and 3D renderers
	Renderer::init();
	Renderer2D::init();
}

void RenderCommand::shutdown() {
	Renderer::shutdown();
	Renderer2D::shutdown();
}

// ----------------------------------------------------------------
// ----- Viewports ------------------------------------------------
// ----------------------------------------------------------------

void RenderCommand::setViewport(const unsigned x, const unsigned y, const unsigned w, const unsigned h) {
	glViewport(x, y, w, h);
	glScissor(x, y, w, h);
}

// ----------------------------------------------------------------
// ----- Screen clearing ------------------------------------------
// ----------------------------------------------------------------

void RenderCommand::setClearColour(const glm::vec4& colour) {
	glClearColor(colour.r, colour.g, colour.b, colour.a);
}

void RenderCommand::setClearColour(const float r, const float g, const float b, const float a) {
	glClearColor(r, g, b, a);
}

void RenderCommand::clearScreen(ClearFlags flags) {
	glClear(static_cast<GLbitfield>(flags));
}

// ----------------------------------------------------------------
// ----- Draw calls -----------------------------------------------
// ----------------------------------------------------------------

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

// ----------------------------------------------------------------
// ----- Face culling ---------------------------------------------
// ----------------------------------------------------------------

void RenderCommand::enableCulling() {
	glEnable(GL_CULL_FACE);
}

void RenderCommand::disableCulling() {
	glDisable(GL_CULL_FACE);
}

// ----------------------------------------------------------------
// ----- Polygon mode ---------------------------------------------
// ----------------------------------------------------------------

void RenderCommand::setDrawMode(DrawMode mode) {
	glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(mode));
}

// ----------------------------------------------------------------
// ----- Depth testing --------------------------------------------
// ----------------------------------------------------------------

void RenderCommand::enableDepthTesting() {
	glEnable(GL_DEPTH_TEST);
}

void RenderCommand::disableDepthTesting() {
	glDisable(GL_DEPTH_TEST);
}

void RenderCommand::enableDepthMask() {
	glDepthMask(GL_TRUE);
}

void RenderCommand::disableDepthMask() {
	glDepthMask(GL_FALSE);
}

void RenderCommand::setDepthTestFunction(DepthTestFunction func) {
	glDepthFunc(static_cast<GLenum>(func));
}

// ----------------------------------------------------------------
// ----- Stencil testing ------------------------------------------
// ----------------------------------------------------------------


void RenderCommand::enableStencilTesting() {
	glEnable(GL_STENCIL_TEST);
}

void RenderCommand::disableStencilTesting() {
	glDisable(GL_STENCIL_TEST);
}

void RenderCommand::enableStencilMask() {
	glStencilMask(0xFF);
}

void RenderCommand::disableStencilMask() {
	glStencilMask(0x00);
}

void RenderCommand::setStencilMask(const unsigned mask) {
	glStencilMask(mask);
}

void RenderCommand::setStencilTestFunction(StencilTestFunction func, const int ref, const unsigned mask) {
	glStencilFunc(static_cast<GLenum>(func), ref, mask);
}

void RenderCommand::setStencilOperation(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation bothPass) {
	glStencilOp(static_cast<GLenum>(stencilFail), static_cast<GLenum>(depthFail), static_cast<GLenum>(bothPass));
}