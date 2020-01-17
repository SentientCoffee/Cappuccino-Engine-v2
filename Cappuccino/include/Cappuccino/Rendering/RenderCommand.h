#pragma once

#include "Cappuccino/Rendering/VertexArray.h"

#include <glm/glm.hpp>

namespace Capp {

	enum class DrawMode : unsigned int {
		// From glad.h
		Point = 0x1B00,		// GL_POINT
		Line = 0x1B01,		// GL_LINE
		Fill = 0x1B02		// GL_FILL
	};
	
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

		static void drawArray(VertexArray* vertexArray);
		static void drawIndexed(VertexArray* vertexArray);

	};
	
}
