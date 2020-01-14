#pragma once

#include "Cappuccino/Rendering/TextureFormats.h"
#include "Cappuccino/Rendering/TextureParams.h"

#include <string>

namespace Capp {

	struct TextureFormats {
		InternalFormat internalFormat = InternalFormat::None;
		PixelFormat pixelFormat = PixelFormat::None;
		PixelType pixelType = PixelType::UnsignedByte;
	};

	struct TextureParams {
		WrapMode wrapS, wrapT, wrapR;

		MinFilter minFilter;
		MagFilter magFilter;

		Mipmaps enableMipmaps;
		Anisotropy anisotropyEnabled;

		TextureParams() = default;
		TextureParams(WrapMode wrapMode, MinFilter min, MagFilter mag, Mipmaps mipmaps = Mipmaps::On, Anisotropy anisotropy = Anisotropy::On);
		TextureParams(WrapMode s, WrapMode t, WrapMode r, MinFilter min, MagFilter mag, Mipmaps mipmaps = Mipmaps::On, Anisotropy anisotropy = Anisotropy::On);
	};
	
	class Texture2D {
	public:

		Texture2D() = default;
		Texture2D(unsigned width, unsigned height, void* data, unsigned channels = 4);
		Texture2D(const std::string& filepath);
		~Texture2D();

		unsigned int getWidth() const;
		unsigned int getHeight() const;

		void bind(unsigned slot = 0) const;
		static void unbind(unsigned slot = 0);

		void setData(void* data, unsigned size);
		
		void setParameters(const TextureParams& params);

	private:

		unsigned _id = 0;
		unsigned _width = 0, _height = 0;
		unsigned _mipLevels;
		unsigned char* _imageData = nullptr;
		
		std::string _texturePath;

		TextureFormats _formats;
		TextureParams _parameters;
		
	};
	
}
