#pragma once
#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Rendering/Buffers/Buffers.h"

namespace Capp {

	class AssetLoader {
	public:

		struct LUTData {
			uint32_t size;
			std::vector<glm::vec3> data;
		};

		struct MeshData {
			Ref<VertexBuffer> vbo;
			Ref<IndexBuffer> ibo;
		};
		
		struct ImageData {
			uint8_t* data;
			uint32_t width, height, channels;
		};
		
		static LUTData loadCUBEFile(const std::string& filepath);
		static ImageData loadImageFile(const std::string& filepath, bool flipped = true, int desiredChannels = 0);
		static MeshData loadOBJFile(const std::string& filepath);
		
		static std::string readTextFile(const std::string& filepath);

	};
	
}
