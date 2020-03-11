#include "CappPCH.h"
#include "Cappuccino/Rendering/Textures/Texture1D.h"
#include "Cappuccino/Resource/AssetLoader.h"

#include <glad/glad.h>

using namespace Capp;

Texture1D::Texture1D(const std::string& filepath) :
	_texturePath(filepath)
{
	const AssetLoader::ImageData image = AssetLoader::loadImageFile(filepath);
	_size = image.width;

	switch(image.channels) {
		case 1:
			_formats = { InternalFormat::Red8, PixelFormat::Red };
			break;
		case 2:
			_formats = { InternalFormat::RG8, PixelFormat::RG };
			break;
		case 3:
			_formats = { InternalFormat::RGB8, PixelFormat::RGB };
			break;
		case 4:
			_formats = { InternalFormat::RGBA8, PixelFormat::RGBA };
			break;
		default:
			_formats = { InternalFormat::None, PixelFormat::None };
			break;
	}

	CAPP_ASSERT(_formats.internalFormat != InternalFormat::None, "Unsupported image format!");
	CAPP_ASSERT(_formats.pixelFormat != PixelFormat::None, "Unsupported image format!");

	glCreateTextures(GL_TEXTURE_1D, 1, &_id);

	glTextureStorage1D(_id, 1, static_cast<GLenum>(_formats.internalFormat), _size);
	glTextureSubImage1D(_id, 0, 0, _size,
		static_cast<GLenum>(_formats.pixelFormat), static_cast<GLenum>(_formats.pixelType), image.data);

	setParameters(_parameters);
}

Texture1D::Texture1D(const unsigned size, void* data, const InternalFormat format) :
	_size(size)
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
		default:
			_formats.pixelFormat = PixelFormat::None;
			break;
	}
	
	_formats.internalFormat = format;

	CAPP_ASSERT(_formats.internalFormat != InternalFormat::None, "Unsupported image format!");
	CAPP_ASSERT(_formats.pixelFormat != PixelFormat::None, "Unsupported image format!");

	glCreateTextures(GL_TEXTURE_1D, 1, &_id);
	
	glTextureStorage1D(_id, 1, static_cast<GLenum>(_formats.internalFormat), _size);

	if(data != nullptr) {
		glTextureSubImage1D(_id, 0, 0, _size,
			static_cast<GLenum>(_formats.pixelFormat), static_cast<GLenum>(_formats.pixelType), data);
	}
	
	setParameters(_parameters);
}

Texture1D::Texture1D(const unsigned size, const InternalFormat format) :
	Texture1D(size, nullptr, format) {}

Texture1D::~Texture1D() {
	glDeleteTextures(1, &_id);
}

unsigned Texture1D::getRendererID() const { return _id; }
unsigned Texture1D::getSize() const { return _size; }

void Texture1D::bind(const unsigned slot) const {
	glBindTextureUnit(slot, _id);
}
void Texture1D::unbind(const unsigned slot) {
	glBindTextureUnit(slot, 0);
}

void Texture1D::setParameters(const TextureParams& params) {
	_parameters = params;

	glTextureParameteri(_id, GL_TEXTURE_WRAP_S, static_cast<GLenum>(_parameters.wrapS));
	glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(_parameters.minFilter));
	glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(_parameters.magFilter));
	glTextureParameterf(_id, GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(_parameters.anisotropyLevel));
	glTextureParameterfv(_id, GL_TEXTURE_BORDER_COLOR, value_ptr(_parameters.borderColour));
}
