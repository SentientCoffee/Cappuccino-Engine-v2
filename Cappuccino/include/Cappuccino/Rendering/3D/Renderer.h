#pragma once

#include "Cappuccino/Rendering/VertexArray.h"
#include "Cappuccino/Rendering/3D/Mesh.h"

namespace Capp {

	class Renderer {
	public:

		static void init();
		static void shutdown();
		
		static void start();
		static void finish();

		static void addToRenderList(VertexArray* vertexArray);
		static void addToRenderList(Mesh* mesh);
		
	};
	
}