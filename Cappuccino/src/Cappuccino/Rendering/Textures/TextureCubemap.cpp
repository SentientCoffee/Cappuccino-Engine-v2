#include "CappPCH.h"
#include "Cappuccino/Rendering/Textures/TextureCubemap.h"

#include "Cappuccino/Rendering/Textures/TextureDefaults.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

using namespace Capp;

TextureCubemap::TextureCubemap(const unsigned faceSize, const InternalFormat format) :
	_size(faceSize)
{
	switch(format) {
		case InternalFormat::Red16F:
		case InternalFormat::Red32F:
			_formats.pixelType = PixelType::Float;
		case InternalFormat::Red8:
		case InternalFormat::Red16:
			_formats.pixelFormat = PixelFormat::Red;
			break;

		case InternalFormat::RG16F:
		case InternalFormat::RG32F:
			_formats.pixelType = PixelType::Float;
		case InternalFormat::RG8:
		case InternalFormat::RG16:
			_formats.pixelFormat = PixelFormat::RG;
			break;

		case InternalFormat::RGB16F:
		case InternalFormat::RGB32F:
			_formats.pixelType = PixelType::Float;
		case InternalFormat::RGB8:
		case InternalFormat::RGB16:
			_formats.pixelFormat = PixelFormat::RGB;
			break;

		case InternalFormat::RGBA16F:
		case InternalFormat::RGBA32F:
			_formats.pixelType = PixelType::Float;
		case InternalFormat::RGBA8:
		case InternalFormat::RGBA16:
			_formats.pixelFormat = PixelFormat::RGBA;
			break;

		case InternalFormat::Depth32F:
			_formats.pixelType = PixelType::Float;
		case InternalFormat::Depth16:
		case InternalFormat::Depth24:
		case InternalFormat::Depth32:
			_formats.pixelFormat = PixelFormat::Depth;
			break;

		case InternalFormat::Stencil4:
		case InternalFormat::Stencil8:
		case InternalFormat::Stencil16:
			_formats.pixelType = PixelType::UnsignedInt;
			_formats.pixelFormat = PixelFormat::Stencil;
			break;

		case InternalFormat::Depth24Stencil8:
			_formats.pixelType = PixelType::UInt24_UInt8;
			_formats.pixelFormat = PixelFormat::DepthStencil;
			break;

		default:
			_formats.pixelFormat = PixelFormat::None;
			break;
	}

	_formats.internalFormat = format;
	
	CAPP_ASSERT(_formats.internalFormat != InternalFormat::None, "Unsupported image format!");
	CAPP_ASSERT(_formats.pixelFormat != PixelFormat::None, "Unsupported pixel format!");

	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &_id);
	glTextureStorage2D(_id, 1, static_cast<GLenum>(_formats.internalFormat), _size, _size);
	
	const TextureParams params = { WrapMode::ClampToEdge, MinFilter::Linear, MagFilter::Linear };
	setParameters(params);
}

TextureCubemap::TextureCubemap(const std::vector<std::string>& filepaths) {
	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &_id);
	setCubemapTextures(filepaths);
}

TextureCubemap::TextureCubemap(const std::initializer_list<std::string>& filepaths) :
	TextureCubemap(std::vector<std::string>(filepaths)) {}

TextureCubemap::~TextureCubemap() {
	glDeleteTextures(1, &_id);
}

unsigned TextureCubemap::getRendererID() const { return _id; }

void TextureCubemap::bind(const unsigned slot) const { glBindTextureUnit(slot, _id); }
void TextureCubemap::unbind(const unsigned slot) { glBindTextureUnit(slot, 0); }

void TextureCubemap::setCubemapTextures(const std::initializer_list<std::string>& filepaths) {
	setCubemapTextures(std::vector<std::string>(filepaths));
}

void TextureCubemap::setCubemapTextures(const std::vector<std::string>& filepaths) {
	CAPP_ASSERT(filepaths.size() <= 6, "Cubemap cannot load in more than 6 faces!");

	stbi_set_flip_vertically_on_load(false);
	for(unsigned i = 0; i < filepaths.size(); ++i) {
		int width, height, channels;
		unsigned char* data = stbi_load(filepaths[i].c_str(), &width, &height, &channels, 0);

		if(!data) {
			CAPP_ASSERT(data != nullptr, "Failed to load texture for texture cube!\n\tTexture path: {0}", filepaths[i]);
			stbi_image_free(data);
		}
		
		if(_size != 0 && (static_cast<unsigned>(width) != _size || static_cast<unsigned>(height) != _size)) {
			CAPP_ASSERT(_size == 0 || static_cast<unsigned>(width) == _size && static_cast<unsigned>(height) == _size, "Texture image file dimensions do not match the size of this cubemap!\n\tTexture path: {0}", filepaths[i]);
			stbi_image_free(data);
		}

		if(width != height) {
			CAPP_ASSERT(width == height, "Texture image for cubemap must be square!\n\tTexture path: {0}", filepaths[i]);
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
				_formats.internalFormat = InternalFormat::Red8;
				_formats.pixelFormat = PixelFormat::Red;
				break;
			case 2:
				_formats.internalFormat = InternalFormat::RG8;
				break;
			case 3:
				_formats.internalFormat = InternalFormat::RGB8;
				_formats.pixelFormat = PixelFormat::RGB;
				break;
			case 4:
				_formats.internalFormat = InternalFormat::RGBA8;
				_formats.pixelFormat = PixelFormat::RGBA;
				break;
			default:
				_formats.internalFormat = InternalFormat::None;
				_formats.pixelFormat = PixelFormat::None;
				break;
		}

		CAPP_ASSERT(_formats.internalFormat != InternalFormat::None, "Unsupported image format!");
		CAPP_ASSERT(_formats.pixelFormat != PixelFormat::None, "Unsupported pixel format!");

		glTexSubImage2D(static_cast<unsigned>(CubemapFace::PositiveX) + i, 0, 0, 0, _size, _size,
			static_cast<GLenum>(_formats.pixelFormat), static_cast<GLenum>(_formats.pixelType), data);

		stbi_image_free(data);
	}

	for(unsigned i = 5; i >= filepaths.size(); --i) {
		_formats.pixelFormat = PixelFormat::RGBA;

		glTexSubImage2D(static_cast<unsigned>(CubemapFace::PositiveX) + i, 0, 0, 0, 1, 1,
			static_cast<GLenum>(_formats.pixelFormat), static_cast<GLenum>(_formats.pixelType), &whiteTexture);
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
