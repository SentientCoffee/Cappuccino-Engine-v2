#include "CappPCH.h"
#include "Cappuccino/Rendering/Textures/Texture2D.h"

#include <glad/glad.h>
#include <glm/gtc/integer.hpp>
#include <stb/stb_image.h>

using namespace Capp;

TextureParams::TextureParams(const WrapMode wrapMode, const MinFilter minFilter, const MagFilter magFilter, const Anisotropy anisotropyLevel, const glm::vec4& borderColour) :
	wrapS(wrapMode), wrapT(wrapMode), wrapR(wrapMode),
	minFilter(minFilter), magFilter(magFilter),
	anisotropyLevel(anisotropyLevel), borderColour(borderColour) {}

TextureParams::TextureParams(const WrapMode wrapS, const WrapMode wrapT, const WrapMode wrapR, const MinFilter minFilter, const MagFilter magFilter, const Anisotropy anisotropyLevel, const glm::vec4& borderColour) :
	wrapS(wrapS), wrapT(wrapT), wrapR(wrapR),
	minFilter(minFilter), magFilter(magFilter),
	anisotropyLevel(anisotropyLevel), borderColour(borderColour) {}

Texture2D::Texture2D(const unsigned width, const unsigned height, void* data, const InternalFormat format, const Mipmaps enableMipmaps) :
	_width(width), _height(height), _formats({}), _parameters({})
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
			_formats.pixelFormat = PixelFormat::Stencil;
			break;

		case InternalFormat::Depth32F_Stencil8:
			_formats.pixelType = PixelType::Float;
		case InternalFormat::Depth24_Stencil8:
			_formats.pixelFormat = PixelFormat::DepthStencil;
			break;

		default:
			_formats.pixelFormat = PixelFormat::None;
			break;
	}

	_formats.internalFormat = format;

	CAPP_ASSERT(_formats.internalFormat != InternalFormat::None, "Unsupported image format!");
	CAPP_ASSERT(_formats.pixelFormat != PixelFormat::None, "Unsupported pixel format!");

	if(enableMipmaps == Mipmaps::On) {
		_mipLevels = glm::max(glm::log2(glm::min(_width, _height)), 1u);
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &_id);
	glTextureStorage2D(_id, _mipLevels > 0 ? _mipLevels : 1u, static_cast<GLenum>(_formats.internalFormat), _width, _height);

	if(data != nullptr) {
		glTextureSubImage2D(_id, 0, 0, 0, _width, _height,
			static_cast<GLenum>(_formats.pixelFormat), static_cast<GLenum>(_formats.pixelType), data);
		if(enableMipmaps == Mipmaps::On) {
			glGenerateTextureMipmap(_id);
		}
	}

	setParameters(_parameters);
}

Texture2D::Texture2D(const unsigned width, const unsigned height, const InternalFormat format, const Mipmaps enableMipmaps) :
	Texture2D(width, height, nullptr, format, enableMipmaps) {}

Texture2D::Texture2D(const std::string& filepath, const Mipmaps enableMipmaps) :
	_formats({}), _parameters({})
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
	CAPP_ASSERT(data != nullptr && width > 0 && height > 0 && channels > 0, "Failed to load image!\n\tTexture path: {0}", filepath);
	_width = width;
	_height = height;

	switch(channels) {
		case 1:
			_formats.internalFormat = InternalFormat::Red8;
			_formats.pixelFormat = PixelFormat::Red;
			break;
		case 2:
			_formats.internalFormat = InternalFormat::RG8;
			_formats.pixelFormat = PixelFormat::RG;
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

	if(enableMipmaps == Mipmaps::On) { _mipLevels = glm::max(glm::log2(glm::min(_width, _height)), 1u); }

	glCreateTextures(GL_TEXTURE_2D, 1, &_id);
	glTextureStorage2D(_id, _mipLevels > 0 ? _mipLevels : 1, static_cast<GLenum>(_formats.internalFormat), _width, _height);
	setParameters(_parameters);

	if(data != nullptr) {
		glTextureSubImage2D(_id, 0, 0, 0, _width, _height,
			static_cast<GLenum>(_formats.pixelFormat), static_cast<GLenum>(_formats.pixelType), data);
	}

	if(enableMipmaps == Mipmaps::On) { glGenerateTextureMipmap(_id); }

	stbi_image_free(data);
}

Texture2D::~Texture2D() {
	glDeleteTextures(1, &_id);
}

unsigned Texture2D::getRendererID() const { return _id; }
unsigned Texture2D::getWidth() const { return _width; }
unsigned Texture2D::getHeight() const { return _height; }
glm::vec2 Texture2D::getSize() const { return { _width, _height }; }

void Texture2D::bind(const unsigned slot) const {
	glBindTextureUnit(slot, _id);
}

void Texture2D::unbind(const unsigned slot) {
	glBindTextureUnit(slot, 0);
}

void Texture2D::setData(void* data, const unsigned size) {
	unsigned bytesPerPixel;
	switch(_formats.pixelFormat) {
		case PixelFormat::Red:
			bytesPerPixel = 1;
			break;
		case PixelFormat::RG:
			bytesPerPixel = 2;
			break;
		case PixelFormat::RGB:
			bytesPerPixel = 3;
			break;
		case PixelFormat::RGBA:
			bytesPerPixel = 4;
			break;
		default:
			bytesPerPixel = 0;
			break;
	}
	CAPP_ASSERT(size == _width * _height * bytesPerPixel, "Data must cover entire texture!");

	glTextureSubImage2D(_id, 0, 0, 0, _width, _height,
		static_cast<GLenum>(_formats.pixelFormat), static_cast<GLenum>(_formats.pixelType), data);
}

void Texture2D::setParameters(const TextureParams& params) {
	_parameters = params;

	glTextureParameteri(_id, GL_TEXTURE_WRAP_S, static_cast<GLenum>(_parameters.wrapS));
	glTextureParameteri(_id, GL_TEXTURE_WRAP_T, static_cast<GLenum>(_parameters.wrapT));
	glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(_parameters.minFilter));
	glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(_parameters.magFilter));
	glTextureParameterf(_id, GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(_parameters.anisotropyLevel));
	glTextureParameterfv(_id, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(_parameters.borderColour));
}
