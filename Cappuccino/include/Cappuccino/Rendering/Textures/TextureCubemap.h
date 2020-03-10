#pragma once

#include "Cappuccino/Rendering/Textures/TextureFormats.h"
#include "Cappuccino/Rendering/Textures/TextureParams.h"

#include <string>
#include <vector>

namespace Capp {

	enum class CubemapFace : unsigned int {
		PositiveX = 0x8515,		// GL_TEXTURE_CUBE_MAP_POSITIVE_X
		NegativeX = 0x8516,		// GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		PositiveY = 0x8517,		// GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		NegativeY = 0x8518,		// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
		PositiveZ = 0x8519,		// GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		NegativeZ = 0x851A,		// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	
	class TextureCubemap {

		using StringVector = std::vector<std::string>;
		using StringInitList = std::initializer_list<std::string>;
		
	public:

		TextureCubemap(unsigned faceSize, InternalFormat format = InternalFormat::RGB8);
		TextureCubemap(const StringVector& filepaths);
		TextureCubemap(const StringInitList& filepaths);
		
		~TextureCubemap();

		void bind(unsigned slot) const;
		unsigned getRendererID() const;
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
