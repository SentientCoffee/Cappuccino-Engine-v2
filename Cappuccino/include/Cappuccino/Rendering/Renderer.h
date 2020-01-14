#pragma once

#include "Cappuccino/Rendering/VertexArray.h"
#include "Cappuccino/Rendering/Mesh.h"

#include <glm/glm.hpp>

namespace Capp {

	class Renderer {
	public:
		
		static void start();
		static void finish();

		static void addToRenderList(VertexArray* vertexArray);
		static void addToRenderList(Mesh* mesh);

	private:

		struct RenderData {
			glm::mat4 viewProjection;
		};

		static RenderData* _renderData;
		
	};
	
}