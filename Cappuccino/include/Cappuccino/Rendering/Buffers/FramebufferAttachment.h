#pragma once

#include "Cappuccino/Rendering/Textures/Texture2D.h"

namespace Capp {
	
	enum class AttachmentTarget : unsigned int {
		None                = 0,
		// From glad.h
		Colour0             = 0x8CE0,		// GL_COLOR_ATTACHMENT0
		Colour1             = 0x8CE1,		// GL_COLOR_ATTACHMENT1
		Colour2             = 0x8CE2,		// GL_COLOR_ATTACHMENT2
		Colour3             = 0x8CE3,		// GL_COLOR_ATTACHMENT3
		Colour4             = 0x8CE4,		// GL_COLOR_ATTACHMENT4
		Colour5             = 0x8CE5,		// GL_COLOR_ATTACHMENT5
		Colour6             = 0x8CE6,		// GL_COLOR_ATTACHMENT6
		Colour7             = 0x8CE7,		// GL_COLOR_ATTACHMENT7
		Depth               = 0x8D00,		// GL_DEPTH_ATTACHMENT
		Stencil             = 0x8D20,		// GL_STENCIL_ATTACHMENT
		DepthStencil        = 0x821A		// GL_DEPTH_STENCIL_ATTACHMENT
	};

	enum class AttachmentFormat : unsigned int {
		// From glad.h
		Red8                = 0x8229,		// GL_R8
		RG8                 = 0x822B,		// GL_RG8
		RGB8                = 0x8051,		// GL_RGB8
		RGBA8               = 0x8058,		// GL_RGBA8
		Depth16             = 0x81A5,		// GL_DEPTH_COMPONENT16
		Depth24             = 0x81A6,		// GL_DEPTH_COMPONENT24
		Depth32             = 0x81A7,		// GL_DEPTH_COMPONENT32
		Stencil4            = 0x8D47,		// GL_STENCIL_INDEX4
		Stencil8            = 0x8D48,		// GL_STENCIL_INDEX8
		Stencil16           = 0x8D49,		// GL_STENCIL_INDEX16
		Depth24Stencil8     = 0x88F0		// GL_DEPTH24_STENCIL8
	};

	enum class AttachmentType : unsigned int {
		Texture = 0,
		RenderBuffer
	};
	
	struct Attachment {
		unsigned id = 0;
		AttachmentType type = AttachmentType::Texture;
		AttachmentFormat format = AttachmentFormat::RGB8;
		Texture2D* texture = nullptr;

		Attachment() = default;
		Attachment(const AttachmentType type, const AttachmentFormat format) :
			type(type), format(format) {}
	};
	
}
