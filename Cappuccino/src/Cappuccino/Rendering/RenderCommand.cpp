#include "CappPCH.h"
#include "Cappuccino/Rendering/RenderCommand.h"

#include "Cappuccino/Rendering/3D/Renderer.h"
#include "Cappuccino/Rendering/2D/Renderer2D.h"

#include <glad/glad.h>

using namespace Capp;

void RenderCommand::init() {
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
 
void RenderCommand::setViewport(const glm::vec2& offset, const glm::vec2& dimensions) {
	glViewport(static_cast<GLint>(offset.x), static_cast<GLint>(offset.y), static_cast<GLsizei>(dimensions.x), static_cast<GLsizei>(dimensions.y));
	glScissor(static_cast<GLint>(offset.x), static_cast<GLint>(offset.y), static_cast<GLsizei>(dimensions.x), static_cast<GLsizei>(dimensions.y));
}

void RenderCommand::setViewport(const uint32_t x, const uint32_t y, const uint32_t w, const uint32_t h) {
	glViewport(x, y, w, h);
	glScissor(x, y, w, h);
}

glm::vec4 RenderCommand::getClearColour() {
	glm::vec4 clearColour;
	glGetFloatv(GL_COLOR_CLEAR_VALUE, glm::value_ptr(clearColour));
	return clearColour;
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

void RenderCommand::drawArray(const Ref<VertexArray>& vertexArray) {
	uint32_t count = 0;
	for(const auto& vbo : vertexArray->getVertexBuffers()) {
		count += vbo->getCount();
	}

	glDrawArrays(GL_TRIANGLES, 0, count);
}

void RenderCommand::drawIndexed(const Ref<VertexArray>& vertexArray) {
	glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}

// ----------------------------------------------------------------
// ----- Face culling ---------------------------------------------
// ----------------------------------------------------------------

void RenderCommand::setCullingMode(CullMode mode) {
	if(mode == CullMode::None) {
		glDisable(GL_CULL_FACE);
	}
	else {
		glEnable(GL_CULL_FACE);
		glCullFace(static_cast<GLenum>(mode));
	}
}

// ----------------------------------------------------------------
// ----- Polygon mode ---------------------------------------------
// ----------------------------------------------------------------

void RenderCommand::setDrawMode(DrawMode mode) {
	glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(mode));
}

void RenderCommand::enableBlending() {
	glEnable(GL_BLEND);
}

void RenderCommand::disableBlending() {
	glDisable(GL_BLEND);
}

void RenderCommand::setBlendFunction(SourceFactor sourceFactor, DestinationFactor destinationFactor) {
	glBlendFunc(static_cast<GLenum>(sourceFactor), static_cast<GLenum>(destinationFactor));
}

void RenderCommand::setSeparateBlendFunction(SourceFactor colourSourceFactor, DestinationFactor colourDestinationFactor, SourceFactor alphaSourceFactor, DestinationFactor alphaDestinationFactor) {
	glBlendFuncSeparate(static_cast<GLenum>(colourSourceFactor), static_cast<GLenum>(colourDestinationFactor), static_cast<GLenum>(alphaSourceFactor), static_cast<GLenum>(alphaDestinationFactor));
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

void RenderCommand::setStencilMask(const uint32_t mask) {
	glStencilMask(mask);
}

void RenderCommand::setStencilTestFunction(StencilTestFunction func, const int ref, const uint32_t mask) {
	glStencilFunc(static_cast<GLenum>(func), ref, mask);
}

void RenderCommand::setStencilOperation(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation bothPass) {
	glStencilOp(static_cast<GLenum>(stencilFail), static_cast<GLenum>(depthFail), static_cast<GLenum>(bothPass));
}