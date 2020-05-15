#pragma once

#include "Cappuccino/Rendering/RenderParams.h"
#include "Cappuccino/Rendering/VertexArray.h"

#include <glm/glm.hpp>

namespace Capp {
	
	class RenderCommand {
	public:

		static void init();
		static void shutdown();
		
		static void setViewport(const glm::vec2& offset, const glm::vec2& dimensions);
		static void setViewport(unsigned x, unsigned y, unsigned w, unsigned h);

		static glm::vec4 getClearColour();
		static void setClearColour(const glm::vec4& colour);
		static void setClearColour(float r, float g, float b, float a);

		static void clearScreen(ClearFlags flags = ClearFlags::All);

		static void drawArray(const Ref<VertexArray>& vertexArray);
		static void drawIndexed(const Ref<VertexArray>& vertexArray);

		static void setCullingMode(CullMode mode);
		static void setDrawMode(DrawMode mode);

		static void enableBlending();
		static void disableBlending();
		static void setBlendFunction(SourceFactor sourceFactor, DestinationFactor destinationFactor);
		static void setSeparateBlendFunction(SourceFactor colourSourceFactor, DestinationFactor colourDestinationFactor, SourceFactor alphaSourceFactor, DestinationFactor alphaDestinationFactor);
		
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
