#pragma once

#include "Cappuccino/Rendering/VertexArray.h"
#include "Cappuccino/Rendering/RenderParams.h"

#include <glm/glm.hpp>

namespace Capp {
	
	class RenderCommand {
	public:

		static void init();
		static void shutdown();
		
		static void setViewport(unsigned x, unsigned y, unsigned w, unsigned h);

		static void setClearColour(const glm::vec4& colour);
		static void setClearColour(float r, float g, float b, float a);
		static void clearScreen();

		static void enableCulling();
		static void disableCulling();

		static void setDrawMode(DrawMode mode);

		static void enableDepthMask();
		static void disableDepthMask();
		static void setDepthTestFunction(DepthTestFunction func);

		static void drawArray(VertexArray* vertexArray);
		static void drawIndexed(VertexArray* vertexArray);

	};
	
}
