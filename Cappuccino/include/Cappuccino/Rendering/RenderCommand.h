#pragma once

#include "Cappuccino/Rendering/VertexArray.h"

#include <glm/glm.hpp>

namespace Capp {

	class RenderCommand {
	public:

		static void setClearColour(const glm::vec4& colour);
		static void setClearColour(float r, float g, float b, float a);
		static void clearScreen();

		static void drawArray(VertexArray* vertexArray);
		static void drawIndexed(VertexArray* vertexArray);
		
	};
	
}
