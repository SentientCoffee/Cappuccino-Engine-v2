#include "CappPCH.h"
#include "Cappuccino/Rendering/Textures/Texture3D.h"

#include "Cappuccino/Resource/AssetLoader.h"

#include <glad/glad.h>

using namespace Capp;

Texture3D::Texture3D(const std::string& filepath) :
	_formats({})
{
	const AssetLoader::LUTData lut = AssetLoader::loadCUBEFile(filepath);
	_width = _height = _depth = lut.size;
	_formats = { InternalFormat::RGB8, PixelFormat::RGB, PixelType::Float };

	glCreateTextures(GL_TEXTURE_3D, 1, &_id);
	
	glTextureStorage3D(_id, 1, static_cast<GLenum>(_formats.internalFormat), _width, _height, _depth);

	if(lut.data.data() != nullptr) {
		glTextureSubImage3D(_id, 0, 0, 0, 0, _width, _height, _depth,
			static_cast<GLenum>(_formats.pixelFormat), static_cast<GLenum>(_formats.pixelType), lut.data.data());
	}

	setParameters(_parameters);
}

Texture3D::Texture3D(const uint32_t width, const uint32_t height, const uint32_t depth, void* data, const InternalFormat format) :
	_width(width), _height(height), _depth(depth), _formats({})
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

	glCreateTextures(GL_TEXTURE_3D, 1, &_id);
	glTextureStorage3D(_id, 1, static_cast<GLenum>(_formats.internalFormat), _width, _height, _depth);

	if(data != nullptr) {
		glTextureSubImage3D(_id, 0, 0, 0, 0, _width, _height, _depth,
			static_cast<GLenum>(_formats.pixelFormat), static_cast<GLenum>(_formats.pixelType), data);
	}
	
	setParameters(_parameters);
}

Texture3D::Texture3D(const uint32_t width, const uint32_t height, const uint32_t depth, const InternalFormat format) :
	Texture3D(width, height, depth, nullptr, format) {}

Texture3D::~Texture3D() {
	glDeleteTextures(1, &_id);
}

void Texture3D::bind(const uint32_t slot) const {
	glBindTextureUnit(slot, _id);
}
void Texture3D::unbind(const uint32_t slot) {
	glBindTextureUnit(slot, 0);
}

void Texture3D::setParameters(const TextureParams& params) {
	_parameters = params;

	glTextureParameteri(_id, GL_TEXTURE_WRAP_S, static_cast<GLenum>(_parameters.wrapS));
	glTextureParameteri(_id, GL_TEXTURE_WRAP_T, static_cast<GLenum>(_parameters.wrapT));
	glTextureParameteri(_id, GL_TEXTURE_WRAP_R, static_cast<GLenum>(_parameters.wrapR));
	glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(_parameters.minFilter));
	glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(_parameters.magFilter));
	glTextureParameterf(_id, GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(_parameters.anisotropyLevel));
	glTextureParameterfv(_id, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(_parameters.borderColour));
}

