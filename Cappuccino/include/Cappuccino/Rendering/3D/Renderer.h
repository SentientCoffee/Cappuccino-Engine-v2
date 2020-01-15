#pragma once

#include "Cappuccino/Rendering/Camera.h"
#include "Cappuccino/Rendering/Shader.h"
#include "Cappuccino/Rendering/VertexArray.h"

#include "Cappuccino/Rendering/3D/Material.h"
#include "Cappuccino/Rendering/3D/Mesh.h"
#include "Cappuccino/Rendering/3D/Model.h"

namespace Capp {

	class Renderer {
	public:

		static void init();
		static void shutdown();
		
		static void start();
		static void start(const PerspectiveCamera& camera);
		static void finish();

		static void addToRenderList(Shader* shader, VertexArray* vertexArray);
		static void addToRenderList(Material* material, VertexArray* vertexArray);
		
		static void addToRenderList(Shader* shader, Mesh* mesh);
		static void addToRenderList(Material* material, Mesh* mesh);

		static void addToRenderList(Model* model);
		
	};
	
}
