#include "CappPCH.h"
#include "Cappuccino/Rendering/Textures/TextureCubemap.h"
#include "Cappuccino/Resource/AssetLoader.h"

#include <glad/glad.h>

using namespace Capp;

CubemapFiles::CubemapFiles(const std::string& left, const std::string& right, const std::string& up, const std::string& down, const std::string& front, const std::string& back) :
	files({ left, right, up, down, front, back }) {}

CubemapFiles::CubemapFiles(const std::array<std::string, 6>& filepaths) :
	files(filepaths) {}

CubemapFiles::CubemapFiles(const std::vector<std::string>& filepaths) {
	for(uint32_t i = 0; i < filepaths.size(); ++i) {
		if(i >= 6) {
			CAPP_PRINT_ERROR("Cubemap cannot load in more than 6 faces!\n\tLast filepath: {0}", filepaths[5]);
			break;
		}
		files[i] = filepaths[i];
	}
}

const std::string& CubemapFiles::operator[](const uint32_t index) const {
	CAPP_ASSERT(index < 6, "Cannot access cubemap file with index {0}! (Index out of range)", index);
	return files.at(index);
}

TextureCubemap::TextureCubemap(const uint32_t faceSize, const InternalFormat format) :
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
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
	for(uint32_t i = 0; i < 6; ++i) {
		glTexImage2D(static_cast<GLenum>(CubemapFace::PositiveX) + i, 0, static_cast<GLenum>(_formats.internalFormat), _size, _size, 0,
			static_cast<GLenum>(_formats.pixelFormat), static_cast<GLenum>(_formats.pixelType), nullptr);
	}
	
	const TextureParams params = { WrapMode::ClampToEdge, MinFilter::Linear, MagFilter::Linear };
	setParameters(params);
}

TextureCubemap::TextureCubemap(const CubemapFiles& filepaths) {
	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &_id);
	setCubemapTextures(filepaths);
}

TextureCubemap::~TextureCubemap() {
	glDeleteTextures(1, &_id);
}

void TextureCubemap::bind(const uint32_t slot) const { glBindTextureUnit(slot, _id); }
void TextureCubemap::unbind(const uint32_t slot) { glBindTextureUnit(slot, 0); }

void TextureCubemap::setCubemapTextures(const CubemapFiles& filepaths) {
	const TextureParams params = { WrapMode::ClampToEdge, MinFilter::Linear, MagFilter::Linear };
	setParameters(params);
	
	for(uint32_t i = 0; i < filepaths.size(); ++i) {
		if(filepaths[i].empty()) {
			CAPP_ASSERT(!filepaths[i].empty(), "Could not read filepath on index {0}!", i);
			continue;
		}

		const AssetLoader::ImageData image = AssetLoader::loadImageFile(filepaths[i], false);
			
		if(image.width != image.height) {
			CAPP_ASSERT(image.width == image.height, "Texture image for cubemap must be square!\n\tTexture path: {0}", filepaths[i]);
			free(image.data);
			return;
		}

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
		if(_formats.internalFormat == InternalFormat::None || _formats.pixelFormat == PixelFormat::None) {
			CAPP_ASSERT(_formats.internalFormat != InternalFormat::None, "Unsupported image format!");
			CAPP_ASSERT(_formats.pixelFormat != PixelFormat::None, "Unsupported pixel format!");
			free(image.data);
			return;
		}

		if(i == 0) {
			_size = image.width;
			glTextureStorage2D(_id, 1, static_cast<GLenum>(_formats.internalFormat), _size, _size);
		}
		
		if(static_cast<uint32_t>(image.width) != _size || static_cast<uint32_t>(image.height) != _size) {
			CAPP_ASSERT(_size == 0 || static_cast<uint32_t>(image.width) == _size && static_cast<uint32_t>(image.height) == _size, "Texture image dimensions do not the set size!\n\tTexture path: {0}", filepaths[i]);
			free(image.data);
			return;
		}
			
		glTextureSubImage3D(_id, 0, 0, 0, i, _size, _size, 1,
			static_cast<GLenum>(_formats.pixelFormat), static_cast<GLenum>(_formats.pixelType), image.data);

		free(image.data);
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
