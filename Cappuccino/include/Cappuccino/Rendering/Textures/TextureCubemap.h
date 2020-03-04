#pragma once

#include "Cappuccino/Rendering/Textures/TextureFormats.h"
#include "Cappuccino/Rendering/Textures/TextureParams.h"

#include <string>
#include <vector>

namespace Capp {

	enum class CubemapFace : unsigned int {
		// DO NOT USE OPENGL MACROS.
		// This implementation use glTextureSubImage3D, which needs 0-based indices.
		PositiveX = 0,		// GL_TEXTURE_CUBE_MAP_POSITIVE_X
		NegativeX = 1,		// GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		PositiveY = 2,		// GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		NegativeY = 3,		// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
		PositiveZ = 4,		// GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		NegativeZ = 5,		// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	
	class TextureCubemap {

		using StringVector = std::vector<std::string>;
		using StringInitList = std::initializer_list<std::string>;
		
	public:

		TextureCubemap(const StringVector& filepaths);
		TextureCubemap(const StringInitList& filepaths);
		
		~TextureCubemap();

		void bind(unsigned slot) const;
		static void unbind(unsigned slot);

		void setCubemapTextures(const StringVector& filepaths);
		void setCubemapTextures(const StringInitList& filepaths);
		
	private:

		void setParameters(const TextureParams& params);
		
		unsigned _id = 0;
		unsigned _size = 0;

		TextureParams _parameters;
		TextureFormats _formats;
	};
	
}
