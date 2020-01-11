#pragma once

#include "Cappuccino/Rendering/VertexArray.h"

#include <glm/glm.hpp>

namespace Capp {

	class Renderer {
	public:
		
		static void start();
		static void finish();

		static void addToRenderList(VertexArray* vertexArray);

	private:

		struct RenderData {
			glm::mat4 viewProjection;
		};

		static RenderData* _renderData;
		
	};
	
}