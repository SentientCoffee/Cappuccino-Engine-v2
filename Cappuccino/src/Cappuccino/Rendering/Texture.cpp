#include "CappPCH.h"
#include "Cappuccino/Rendering/Texture.h"

#include <glad/glad.h>
#include <glm/gtc/integer.hpp>
#include <stb/stb_image.h>

using namespace Capp;

TextureParams::TextureParams(const WrapMode wrapMode, const MinFilter min, const MagFilter mag, const Mipmaps mipmaps, const Anisotropy anisotropy) {
	wrapS = wrapT = wrapR = wrapMode;
	minFilter = min;
	magFilter = mag;
	enableMipmaps = mipmaps;
	anisotropyEnabled = anisotropy;
}

TextureParams::TextureParams(const WrapMode s, const WrapMode t, const WrapMode r, const MinFilter min, const MagFilter mag, const Mipmaps mipmaps, const Anisotropy anisotropy) {
	wrapS = s;
	wrapT = t;
	wrapR = r;
	minFilter = min;
	magFilter = mag;
	enableMipmaps = mipmaps;
	anisotropyEnabled = anisotropy;
}

Texture2D::Texture2D(const unsigned width, const unsigned height, void* data, const unsigned channels) :
	_width(width), _height(height), _imageData(static_cast<unsigned char*>(data)), _texturePath(""), _parameters({}) {
	_formats.internalFormat = channels == 4 ? InternalFormat::RGBA8 : InternalFormat::RGB8;
	_formats.pixelFormat = channels == 4 ? PixelFormat::RGBA : PixelFormat::RGB;
	glCreateTextures(GL_TEXTURE_2D, 1, &_id);

	_mipLevels = glm::max(glm::log2(glm::min(_width, _height)), 1u);
	
	// Default texture parameters
	const TextureParams params = {
		WrapMode::Repeat,
		MinFilter::Linear,
		MagFilter::Nearest
	};

	setParameters(params);

	CAPP_ASSERT(_imageData != nullptr, "No image data to create texture!");

	glTextureSubImage2D(_id, 0,
	                    0, 0, _width, _height,
	                    static_cast<GLenum>(_formats.pixelFormat),
	                    static_cast<GLenum>(_formats.pixelType),
	                    _imageData);

	if(static_cast<bool>(_parameters.enableMipmaps)) {
		glGenerateTextureMipmap(_id);
	}
}

Texture2D::Texture2D(const std::string& filepath) :
	_texturePath(filepath), _parameters({}) {
	glCreateTextures(GL_TEXTURE_2D, 1, &_id);
	
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	_imageData = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

	if(_imageData == nullptr) {
		CAPP_PRINT_ERROR("Texture path: {0}", filepath);
		CAPP_ASSERT(_imageData, "Failed to load image!");
	}

	_width = width; _height = height;
	
	if(channels == 4) {
		_formats.internalFormat = InternalFormat::RGBA8;
		_formats.pixelFormat = PixelFormat::RGBA;
	}
	else if(channels == 3) {
		_formats.internalFormat = InternalFormat::RGB8;
		_formats.pixelFormat = PixelFormat::RGB;
	}
	else if(channels == 1) {
		_formats.internalFormat = InternalFormat::R8;
		_formats.pixelFormat = PixelFormat::Red;
	}

	CAPP_ASSERT(_formats.internalFormat != InternalFormat::None || _formats.pixelFormat != PixelFormat::None, "Unsupported image format!");
	
	_mipLevels = glm::max(glm::log2(glm::min(_width, _height)), 1u);

	// Default texture parameters
	const TextureParams params = {
		WrapMode::Repeat,
		MinFilter::Linear,
		MagFilter::Nearest
	};

	setParameters(params);
	
	CAPP_ASSERT(_imageData != nullptr, "No image data to create texture!");

	glTextureSubImage2D(_id, 0,
	                    0, 0, _width, _height,
	                    static_cast<GLenum>(_formats.pixelFormat),
	                    static_cast<GLenum>(_formats.pixelType),
	                    _imageData);

	if(static_cast<bool>(_parameters.enableMipmaps)) {
		glGenerateTextureMipmap(_id);
	}
}

Texture2D::~Texture2D() {
	glDeleteTextures(1, &_id);
	stbi_image_free(_imageData);
}

unsigned Texture2D::getWidth() const { return _width; }
unsigned Texture2D::getHeight() const { return _height; }

void Texture2D::bind(const unsigned slot) const {
	glBindTextureUnit(slot, _id);
}
void Texture2D::unbind(const unsigned slot) {
	glBindTextureUnit(slot, 0);
}

void Texture2D::setData(void* data, const unsigned size) {
	const unsigned bytesPerPixel = _formats.pixelFormat == PixelFormat::RGBA ? 4 : 3;
	CAPP_ASSERT(size == _width * _height * bytesPerPixel, "Data must cover entire texture!");

	_imageData = static_cast<unsigned char*>(data);
	
	CAPP_ASSERT(_imageData != nullptr, "No image data to create texture!");

	glTextureSubImage2D(_id, 0,
	                    0, 0, _width, _height,
	                    static_cast<GLenum>(_formats.pixelFormat),
	                    static_cast<GLenum>(_formats.pixelType),
	                    _imageData);

	if(static_cast<bool>(_parameters.enableMipmaps)) {
		glGenerateTextureMipmap(_id);
	}
}

void Texture2D::setParameters(const TextureParams& params) {
	_parameters = params;
	
	glTextureStorage2D(_id,
	                   static_cast<bool>(_parameters.enableMipmaps) ? _mipLevels : 1,
	                   static_cast<GLenum>(_formats.internalFormat),
	                   _width, _height);

	glTextureParameteri(_id, GL_TEXTURE_WRAP_S, static_cast<GLenum>(_parameters.wrapS));
	glTextureParameteri(_id, GL_TEXTURE_WRAP_T, static_cast<GLenum>(_parameters.wrapT));
	glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(_parameters.minFilter));
	glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(_parameters.magFilter));

	if(static_cast<bool>(_parameters.anisotropyEnabled)) {
		glTextureParameterf(_id, GL_TEXTURE_MAX_ANISOTROPY, 2.0f);
	}
}
