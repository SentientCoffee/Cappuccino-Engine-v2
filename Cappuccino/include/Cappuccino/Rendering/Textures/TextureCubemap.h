#pragma once

#include "Cappuccino/Core/Memory.h"
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

	struct CubemapFiles {
		std::array<std::string, 6> files;
		
		CubemapFiles(const std::string& left, const std::string& right, const std::string& up, const std::string& down, const std::string& front, const std::string& back);
		CubemapFiles(const std::array<std::string, 6>& filepaths);
		CubemapFiles(const std::vector<std::string>& filepaths);

		const std::string& operator[](unsigned index) const;
		unsigned size() const;
	};
	
	class TextureCubemap {
	public:

		TextureCubemap() = default;
		TextureCubemap(unsigned faceSize, InternalFormat format = InternalFormat::RGB8);
		TextureCubemap(const CubemapFiles& filepaths);
		~TextureCubemap();

		static Ref<TextureCubemap> create(unsigned faceSize, InternalFormat format = InternalFormat::RGB8) {
			return Memory::createRef<TextureCubemap>(faceSize, format);
		}
		
		static Ref<TextureCubemap> create(const CubemapFiles& filepaths) {
			return Memory::createRef<TextureCubemap>(filepaths);
		}
		
		void bind(unsigned slot) const;
		unsigned getRendererID() const;
		static void unbind(unsigned slot);

		void setCubemapTextures(const CubemapFiles& filepaths);
		
	private:
		
		void setParameters(const TextureParams& params);
		
		unsigned _id = 0;
		unsigned _size = 0;

		TextureParams _parameters;
		TextureFormats _formats;
	};
	
}
