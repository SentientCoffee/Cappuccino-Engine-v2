#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/TextureCubemap.h"

#include "Cappuccino/Rendering/TextureParams.h"
#include "Cappuccino/Rendering/TextureDefaults.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

using namespace Capp;

TextureCubemap::TextureCubemap(const std::vector<std::string>& filepaths) :
	_parameters({}) {
	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &_id);
	setCubemapTextures(filepaths);
}

TextureCubemap::TextureCubemap(const std::initializer_list<std::string>& filepaths) :
	TextureCubemap(std::vector<std::string>(filepaths)) {}

TextureCubemap::~TextureCubemap() {
	glDeleteTextures(1, &_id);
}

void TextureCubemap::bind(const unsigned slot) const { glBindTextureUnit(slot, _id); }
void TextureCubemap::unbind(const unsigned slot) { glBindTextureUnit(slot, 0); }

void TextureCubemap::setCubemapTextures(const std::initializer_list<std::string>& filepaths) {
	setCubemapTextures(std::vector<std::string>(filepaths));
}

void TextureCubemap::setCubemapTextures(const std::vector<std::string>& filepaths) {
	CAPP_ASSERT(filepaths.size() <= 6, "Cubemap cannot load in more than 6 faces!");
	_formats.internalFormat = InternalFormat::RGB8;

	stbi_set_flip_vertically_on_load(true);
	for(unsigned i = 0; i < filepaths.size(); ++i) {
		int width, height, channels;
		unsigned char* data = stbi_load(filepaths[i].c_str(), &width, &height, &channels, 0);

		if(!data) {
			CAPP_ASSERT(data != nullptr, "Failed to load texture for texture cube!\nTexture path: {0}", filepaths[i]);
			stbi_image_free(data);
		}
		
		if(_size != 0 && (static_cast<unsigned>(width) != _size || static_cast<unsigned>(height) != _size)) {
			CAPP_ASSERT(_size == 0 || static_cast<unsigned>(width) == _size && static_cast<unsigned>(height) == _size, "Texture image file dimensions do not match the size of this cubemap!\nTexture path: {0}", filepaths[i]);
			stbi_image_free(data);
		}

		if(width != height) {
			CAPP_ASSERT(width == height, "Texture image for cubemap must be square!\nTexture path: {0}", filepaths[i]);
			stbi_image_free(data);
		}

		if(i == 0) {
			_size = width;

			const TextureParams params = {
				WrapMode::ClampToEdge,
				MinFilter::Linear,
				MagFilter::Linear
			};

			setParameters(params);
			
			glTextureStorage2D(_id, 1, static_cast<GLenum>(_formats.internalFormat), _size, _size);
		}

		switch(channels) {
			case 1: 
				_formats.pixelFormat = PixelFormat::Red;
				break;
			case 3:
				_formats.pixelFormat = PixelFormat::RGB;
				break;
			case 4:
				_formats.pixelFormat = PixelFormat::RGBA;
				break;
			default:
				_formats.pixelFormat = PixelFormat::None;
				break;
		}

		CAPP_ASSERT(_formats.pixelFormat != PixelFormat::None, "Unsupported image format!");

		glTextureSubImage3D(_id, 0, 0, 0,
		                    static_cast<GLenum>(static_cast<unsigned>(CubemapFace::PositiveX) + i),
		                    _size, _size, 1,
		                    static_cast<GLenum>(_formats.pixelFormat),
		                    static_cast<GLenum>(_formats.pixelType), data);

		stbi_image_free(data);
	}

	for(unsigned i = 5; i >= filepaths.size(); --i) {
		_formats.pixelFormat = PixelFormat::RGBA;

		glTextureSubImage3D(_id, 0,
		                    0, 0, static_cast<int>(CubemapFace::PositiveX) + i,
		                    1, 1, 1,
		                    static_cast<GLenum>(_formats.pixelFormat),
		                    static_cast<GLenum>(_formats.pixelType), &whiteTexture);
	}
}

void TextureCubemap::setParameters(const TextureParams& params) {
	_parameters = params;
	
	glTextureParameteri(_id, GL_TEXTURE_WRAP_S, static_cast<GLenum>(_parameters.wrapS));
	glTextureParameteri(_id, GL_TEXTURE_WRAP_T, static_cast<GLenum>(_parameters.wrapT));
	glTextureParameteri(_id, GL_TEXTURE_WRAP_R, static_cast<GLenum>(_parameters.wrapR));
	glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(_parameters.minFilter));
	glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(_parameters.magFilter));
}
