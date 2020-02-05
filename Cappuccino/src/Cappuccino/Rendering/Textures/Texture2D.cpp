#include "CappPCH.h"
#include "Cappuccino/Rendering/Textures/Texture2D.h"

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
	_width(width), _height(height), _data(static_cast<unsigned char*>(data)), _texturePath(""), _parameters({}) {

	switch(channels) {
		case 1:
			_formats.internalFormat = InternalFormat::R8;
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

	createTexture();
}

Texture2D::Texture2D(const std::string& filepath) :
	_texturePath(filepath), _parameters({}) {
	
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	_data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

	CAPP_ASSERT(_data != nullptr, "Failed to load image!\n\tTexture path: {0}", filepath);

	_width = width; _height = height;
	
	switch(channels) {
		case 1:
			_formats.internalFormat = InternalFormat::R8;
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

	createTexture();
}

Texture2D::~Texture2D() {
	glDeleteTextures(1, &_id);
	if(_data != nullptr) {
		stbi_image_free(_data);
	}
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

	_data = static_cast<unsigned char*>(data);

	glTextureSubImage2D(
		_id, 0,
		0, 0, _width, _height,
		static_cast<GLenum>(_formats.pixelFormat),
		static_cast<GLenum>(_formats.pixelType),
		_data
	);

	if(static_cast<bool>(_parameters.enableMipmaps)) {
		glGenerateTextureMipmap(_id);
	}
}

void Texture2D::setParameters(const TextureParams& params) {
	_parameters = params;

	glTextureParameteri(_id, GL_TEXTURE_WRAP_S, static_cast<GLenum>(_parameters.wrapS));
	glTextureParameteri(_id, GL_TEXTURE_WRAP_T, static_cast<GLenum>(_parameters.wrapT));
	glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(_parameters.minFilter));
	glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(_parameters.magFilter));

	if(static_cast<bool>(_parameters.anisotropyEnabled)) {
		glTextureParameterf(_id, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);
	}
}

void Texture2D::createTexture() {
	CAPP_ASSERT(_formats.internalFormat != InternalFormat::None, "Unsupported image format!");
	CAPP_ASSERT(_formats.pixelFormat != PixelFormat::None, "Unsupported image format!");
	
	glCreateTextures(GL_TEXTURE_2D, 1, &_id);
	
	_mipLevels = glm::max(glm::log2(glm::min(_width, _height)), 1u);

	// Default texture parameters
	const TextureParams params = {
		WrapMode::Repeat,
		MinFilter::Linear,
		MagFilter::Nearest
	};

	setParameters(params);

	glTextureStorage2D(
		_id,
		static_cast<bool>(_parameters.enableMipmaps) ? _mipLevels : 1,
		static_cast<GLenum>(_formats.internalFormat),
		_width, _height
	);

	if(_data != nullptr) {
		glTextureSubImage2D(
			_id, 0,
			0, 0, _width, _height,
			static_cast<GLenum>(_formats.pixelFormat),
			static_cast<GLenum>(_formats.pixelType),
			_data
		);

		if(static_cast<bool>(_parameters.enableMipmaps)) {
			glGenerateTextureMipmap(_id);
		}
	}
}
