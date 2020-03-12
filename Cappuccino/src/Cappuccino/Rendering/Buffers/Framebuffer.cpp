#include "CappPCH.h"
#include "Cappuccino/Rendering/Buffers/Framebuffer.h"

#include <glad/glad.h>

using namespace Capp;

Framebuffer::Framebuffer(const unsigned width, const unsigned height) :
	_width(width), _height(height) {
	CAPP_ASSERT(width > 0 && height > 0, "Width and height must be greater than 0!");
	
	glCreateFramebuffers(1, &_id);
	_name = "Framebuffer " + std::to_string(_id);
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &_id);
}

unsigned Framebuffer::getWidth() const { return _width; }
unsigned Framebuffer::getHeight() const { return _height; }
bool Framebuffer::isValid() const { return _isValid; }

const std::string& Framebuffer::getName() const { return _name; }
void Framebuffer::setName(const std::string& name) { _name = name; }

void Framebuffer::resize(const unsigned width, const unsigned height) {
	CAPP_ASSERT(width > 0 && height > 0, "Width and height must be greater than 0!");

	if(_width == width && _height == height) return;

	_width = width; _height = height;
	for(auto& attachment : _attachments) {
		addAttachment(attachment.first, attachment.second);
	}

	validateFramebuffer();
}

void Framebuffer::addAttachment(const AttachmentTarget target, const Attachment& attachment) {
	const auto it = _attachments.find(target);
	if(it != _attachments.end()) {
		CAPP_PRINT_WARNING("An attachment in framebuffer \"{0}\" is already bound to target \"{1}\", replacing with new attachment...", _name, target);

		if(it->second.type == AttachmentType::RenderBuffer) {
			glDeleteRenderbuffers(1, &it->second.id);
		}
		else {
			it->second.texture = nullptr;
			it->second.cubemap = nullptr;
		}
	}
	else if(target >= AttachmentTarget::Colour0 && target <= AttachmentTarget::Colour7) {
		_drawTargets.push_back(target);
		glNamedFramebufferDrawBuffers(_id, static_cast<GLsizei>(_drawTargets.size()), reinterpret_cast<const GLenum*>(_drawTargets.data()));
	}
	
	Attachment a = attachment;

	switch(a.type) {
		case AttachmentType::RenderBuffer:
			glCreateRenderbuffers(1, &a.id);
			glNamedRenderbufferStorage(a.id, static_cast<GLenum>(a.format), _width, _height);
			glNamedFramebufferRenderbuffer(_id, static_cast<GLenum>(target), GL_RENDERBUFFER, a.id);
			break;
		case AttachmentType::Texture:
			a.texture = new Texture2D(_width, _height, a.format, Mipmaps::Off);
			a.texture->setParameters(a.parameters);
			a.id = a.texture->getRendererID();

			glNamedFramebufferTexture(_id, static_cast<GLenum>(target), a.id, 0);
			break;
		case AttachmentType::Cubemap:
			CAPP_ASSERT(_width == _height, "Texture attachment for cubemap must be square!\n\tFramebuffer: {0}", _name);
			a.cubemap = new TextureCubemap(_width, a.format);
			a.id = a.cubemap->getRendererID();

			glNamedFramebufferTexture(_id, static_cast<GLenum>(target), a.id, 0);
			break;
	}

	_attachments[target] = a;
}

Texture2D* Framebuffer::getTextureAttachment(const AttachmentTarget target) {
	const auto it = _attachments.find(target);
	if(it == _attachments.end()) {
		CAPP_ASSERT(it != _attachments.end(), "Failed to find attachment {0} from framebuffer \"{1}\"!", target, _name);
		return nullptr;
	}
	if(it->second.type == AttachmentType::RenderBuffer) {
		CAPP_ASSERT(it->second.type != AttachmentType::RenderBuffer, "Attachment {0} from framebuffer \"{1}\" is a renderbuffer, cannot retrieve", target, _name);
		return nullptr;
	}
	if(it->second.type == AttachmentType::Cubemap) {
		CAPP_ASSERT(it->second.type != AttachmentType::Cubemap, "Attachment {0} from framebuffer \"{1}\" is a cubemap, try using Framebuffer::getCubemapAttachment()", target, _name);
		return nullptr;
	}
	
	return it->second.texture;
}

TextureCubemap* Framebuffer::getCubemapAttachment(const AttachmentTarget target) {
	const auto it = _attachments.find(target);
	if(it == _attachments.end()) {
		CAPP_ASSERT(it != _attachments.end(), "Failed to find attachment {0} from framebuffer \"{1}\"!", target, _name);
		return nullptr;
	}
	if(it->second.type == AttachmentType::RenderBuffer) {
		CAPP_ASSERT(it->second.type != AttachmentType::RenderBuffer, "Attachment {0} from framebuffer \"{1}\" is a renderbuffer, cannot retrieve", target, _name);
		return nullptr;
	}
	if(it->second.type == AttachmentType::Texture) {
		CAPP_ASSERT(it->second.type != AttachmentType::Texture, "Attachment {0} from framebuffer \"{1}\" is a texture, try using Framebuffer::getTextureAttachment()", target, _name);
		return nullptr;
	}

	return it->second.cubemap;
}

void Framebuffer::bind(const FramebufferBinding binding) {
	validateFramebuffer();
	_binding = binding;
	glBindFramebuffer(static_cast<GLenum>(binding), _id);
}

void Framebuffer::unbind() {
	if(_binding == FramebufferBinding::None) {
		return;
	}

	glBindFramebuffer(static_cast<GLenum>(_binding), 0);
	_binding = FramebufferBinding::None;
}

void Framebuffer::blitBufferData(const glm::ivec4& sourceBounds, const glm::ivec4& destinationBounds, const ClearFlags flags, const MagFilter magFilter) {
	glBlitFramebuffer(sourceBounds.x, sourceBounds.y, sourceBounds.z, sourceBounds.w,
		destinationBounds.x, destinationBounds.y, destinationBounds.z, destinationBounds.w,
		static_cast<GLbitfield>(flags), static_cast<GLenum>(magFilter));
}


bool Framebuffer::validateFramebuffer() {
	const unsigned result = glCheckNamedFramebufferStatus(_id, GL_FRAMEBUFFER);
	if(result == GL_FRAMEBUFFER_COMPLETE) {
		return _isValid = true;
	}

	switch(result) {
		case GL_FRAMEBUFFER_UNDEFINED:
			CAPP_ASSERT(result == GL_FRAMEBUFFER_COMPLETE, "Framebuffer \"{0}\" failed to validate. The default framebuffer is undefined.", _name);
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			CAPP_ASSERT(result == GL_FRAMEBUFFER_COMPLETE,"Framebuffer \"{0}\" failed to validate. One of the attachment points is framebuffer incomplete.", _name);
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			CAPP_ASSERT(result == GL_FRAMEBUFFER_COMPLETE,"Framebuffer \"{0}\" failed to validate. There are no attachments.", _name);
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			CAPP_ASSERT(result == GL_FRAMEBUFFER_COMPLETE,"Framebuffer \"{0}\" failed to validate. Draw buffer is incomplete.", _name);
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			CAPP_ASSERT(result == GL_FRAMEBUFFER_COMPLETE,"Framebuffer \"{0}\" failed to validate. Read buffer is incomplete.", _name);
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			CAPP_ASSERT(result == GL_FRAMEBUFFER_COMPLETE,"Framebuffer \"{0}\" failed to validate. Check the formats of the attached targets.", _name);
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			CAPP_ASSERT(result == GL_FRAMEBUFFER_COMPLETE,"Framebuffer \"{0}\" failed to validate. Check the multisampling parameters on all attached targets.", _name);
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			CAPP_ASSERT(result == GL_FRAMEBUFFER_COMPLETE,"Framebuffer \"{0}\" failed to validate. Check layer targets.", _name);
			break;
		default:
			CAPP_ASSERT(result == GL_FRAMEBUFFER_COMPLETE,"Framebuffer \"{0}\" failed to validate for unknown reason!\n(Common error: check your attachment dimensions?)", _name);
			break;
	}

	return _isValid = false;
}
