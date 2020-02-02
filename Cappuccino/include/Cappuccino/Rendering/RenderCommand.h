#pragma once

#include "Cappuccino/Rendering/RenderParams.h"
#include "Cappuccino/Rendering/VertexArray.h"

#include <glm/glm.hpp>

namespace Capp {
	
	class RenderCommand {
	public:

		static void init();
		static void shutdown();
		
		static void setViewport(unsigned x, unsigned y, unsigned w, unsigned h);

		static glm::vec4 getClearColour();
		static void setClearColour(const glm::vec4& colour);
		static void setClearColour(float r, float g, float b, float a);

		static void clearScreen(ClearFlags flags = ClearFlags::All);

		static void drawArray(VertexArray* vertexArray);
		static void drawIndexed(VertexArray* vertexArray);

		static void enableCulling();
		static void disableCulling();

		static void setDrawMode(DrawMode mode);

		static void enableDepthTesting();
		static void disableDepthTesting();
		static void enableDepthMask();
		static void disableDepthMask();
		static void setDepthTestFunction(DepthTestFunction func);

		static void enableStencilTesting();
		static void disableStencilTesting();
		static void enableStencilMask();
		static void disableStencilMask();
		static void setStencilMask(unsigned mask);
		static void setStencilTestFunction(StencilTestFunction func, int ref = 1, unsigned mask = 0xFF);
		static void setStencilOperation(StencilOperation stencilFail, StencilOperation depthFail, StencilOperation bothPass);

	};
	
}
