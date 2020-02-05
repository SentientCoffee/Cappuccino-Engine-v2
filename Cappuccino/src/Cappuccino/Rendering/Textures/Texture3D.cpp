#include "CappPCH.h"
#include "Cappuccino/Rendering/Textures/Texture3D.h"

#include <glad/glad.h>

using namespace Capp;

Texture3D::Texture3D(const std::string& filepath) :
	_texturePath(filepath), _parameters({}) {
	// TODO: ADD FILEPATH CREATION OF 3D TEXTURES
}

Texture3D::Texture3D(const unsigned width, const unsigned height, const unsigned depth, void* data) :
	_width(width), _height(height), _depth(depth), _data(data), _parameters({}) {

	_formats.internalFormat = InternalFormat::RGB8;
	_formats.pixelFormat = PixelFormat::RGB;
	_formats.pixelType = PixelType::Float;
	
	createTexture();
}

Texture3D::~Texture3D() {
	glDeleteTextures(1, &_id);
}

unsigned Texture3D::getRendererID() const { return _id; }
unsigned Texture3D::getWidth() const { return _width; }
unsigned Texture3D::getHeight() const { return _height; }
unsigned Texture3D::getDepth() const { return _depth; }
glm::vec3 Texture3D::getSize() const { return { _width, _height, _depth }; }

void Texture3D::bind(const unsigned slot) const {
	glBindTextureUnit(slot, _id);
}
void Texture3D::unbind(const unsigned slot) {
	glBindTextureUnit(slot, 0);
}

void Texture3D::setParameters(const TextureParams& params) {
	_parameters = params;

	glTextureParameteri(_id, GL_TEXTURE_WRAP_S, static_cast<GLenum>(_parameters.wrapS));
	glTextureParameteri(_id, GL_TEXTURE_WRAP_T, static_cast<GLenum>(_parameters.wrapT));
	glTextureParameteri(_id, GL_TEXTURE_WRAP_R, static_cast<GLenum>(_parameters.wrapR));
	glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(_parameters.minFilter));
	glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(_parameters.magFilter));
}

void Texture3D::createTexture() {
	CAPP_ASSERT(_formats.internalFormat != InternalFormat::None, "Unsupported image format!");
	CAPP_ASSERT(_formats.pixelFormat != PixelFormat::None, "Unsupported image format!");

	glCreateTextures(GL_TEXTURE_3D, 1, &_id);

	// Default texture parameters
	const TextureParams params = {
		WrapMode::Repeat,
		MinFilter::Linear,
		MagFilter::Linear
	};

	setParameters(params);

	glTextureStorage3D(
		_id, 1,
		static_cast<GLenum>(_formats.internalFormat),
		_width, _height, _depth
	);

	if(_data != nullptr) {
		glTextureSubImage3D(
			_id, 0,
			0, 0, 0, _width, _height, _depth,
			static_cast<GLenum>(_formats.pixelFormat),
			static_cast<GLenum>(_formats.pixelType),
			_data
		);
	}
	
}