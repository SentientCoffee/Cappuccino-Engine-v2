#pragma once

#include "Cappuccino/Rendering/VertexArray.h"

namespace Capp {

	class Renderer {
	public:
		static void beginScene();
		static void endScene();

		static void addToRenderList(VertexArray* vertexArray);
		
	};
	
}