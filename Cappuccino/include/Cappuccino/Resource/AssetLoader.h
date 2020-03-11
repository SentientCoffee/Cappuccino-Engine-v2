#pragma once
#include "Cappuccino/Rendering/Textures/Texture3D.h"
#include "Cappuccino/Rendering/Buffers/Buffers.h"

namespace Capp {

	class AssetLoader {
	public:

		static Texture3D* loadCUBEFile(const std::string& filepath);

		struct MeshData {
			VertexBuffer* vbo;
			IndexBuffer* ibo;
		};

		static MeshData loadOBJFile(const std::string& filepath);
		
		static std::string readTextFile(const std::string& filepath);

		struct ImageData {
			unsigned char* data;
			unsigned width, height, channels;
		};
		
		static ImageData loadImageFile(const std::string& filepath, bool flipped = true, int desiredChannels = 0);
	};
	
}
