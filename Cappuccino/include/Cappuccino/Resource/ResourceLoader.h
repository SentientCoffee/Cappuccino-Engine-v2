#pragma once
#include "Cappuccino/Rendering/Textures/Texture3D.h"
#include "Cappuccino/Rendering/Buffers/Buffers.h"

namespace Capp {

	class ResourceLoader {
	public:

		static Texture3D* loadCUBEFile(const std::string& filepath);
		static std::tuple<VertexBuffer*, IndexBuffer*> loadOBJFile(const std::string& filepath);
		static std::string loadTextFile(const std::string& filepath);
		
	};
	
}
