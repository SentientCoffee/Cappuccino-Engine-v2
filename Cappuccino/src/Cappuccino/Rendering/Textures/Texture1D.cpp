#include "CappPCH.h"
#include "Cappuccino/Rendering/Textures/Texture1D.h"
#include <glad/glad.h>

#include <stb/stb_image.h>

Capp::Texture1D::Texture1D(const std::string& filepath) :
	_texturePath(filepath)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	_data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

	CAPP_ASSERT(_data != nullptr, "Failed to load image!\n\tTexture path: {0}", filepath);

	_size = width;

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

	createTexture();
	
}

Capp::Texture1D::Texture1D(const unsigned size, void* data) :
	_size(size), _data(data) {

	_formats.internalFormat = InternalFormat::Red8;
	_formats.pixelFormat = PixelFormat::Red;

	createTexture();
}

Capp::Texture1D::~Texture1D() {
	glDeleteTextures(1, &_id);
}

unsigned Capp::Texture1D::getRendererID() const { return _id; }
unsigned Capp::Texture1D::getSize() const { return _size; }

void Capp::Texture1D::bind(const unsigned slot) const {
	glBindTextureUnit(slot, _id);
}
void Capp::Texture1D::unbind(const unsigned slot) {
	glBindTextureUnit(slot, 0);
}

void Capp::Texture1D::setParameters(const TextureParams& params) {
	_parameters = params;

	glTextureParameteri(_id, GL_TEXTURE_WRAP_S, static_cast<GLenum>(_parameters.wrapS));
	glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(_parameters.minFilter));
	glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(_parameters.magFilter));
	glTextureParameterf(_id, GL_TEXTURE_MAX_ANISOTROPY, static_cast<float>(_parameters.anisotropyLevel));
	glTextureParameterfv(_id, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(_parameters.borderColour));
}

void Capp::Texture1D::createTexture() {
	CAPP_ASSERT(_formats.internalFormat != InternalFormat::None, "Unsupported image format!");
	CAPP_ASSERT(_formats.pixelFormat != PixelFormat::None, "Unsupported image format!");
	
	glCreateTextures(GL_TEXTURE_1D, 1, &_id);

	// Default texture parameters
	const TextureParams params = {
		WrapMode::Repeat,
		MinFilter::Linear,
		MagFilter::Nearest
	};

	setParameters(params);

	glTextureStorage1D(
		_id, 1,
		static_cast<GLenum>(_formats.internalFormat),
		_size
	);

	if(_data != nullptr) {
		glTextureSubImage1D(
			_id, 0,
			0, _size,
			static_cast<GLenum>(_formats.pixelFormat),
			static_cast<GLenum>(_formats.pixelType),
			_data
		);
	}
}
