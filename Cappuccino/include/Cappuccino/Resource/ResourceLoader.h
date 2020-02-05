#pragma once
#include "Cappuccino/Rendering/Textures/Texture3D.h"
#include "Cappuccino/Rendering/Buffers/Buffers.h"

namespace Capp {

	class ResourceLoader {
	public:

		static Texture3D* loadCUBE(const std::string& filepath);
		static std::tuple<VertexBuffer*, IndexBuffer*> loadOBJ(const std::string& filepath);
		
	};
	
}
