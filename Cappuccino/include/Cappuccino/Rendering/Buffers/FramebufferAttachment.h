#pragma once

#include "Cappuccino/Rendering/Textures/Texture2D.h"

namespace Capp {
	
	enum class AttachmentTarget : unsigned int {
		None = 0,
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

	//enum class AttachmentFormat : unsigned int {
	//	// From glad.h
	//	Red8                = 0x8229,		// GL_R8
	//	Red16               = 0x822A,		// GL_R16
	//	Red16F              = 0x822D,		// GL_R16F
	//	
	//	RG8                 = 0x822B,		// GL_RG8
	//	RG16                = 0x822C,		// GL_RG16
	//	RG16F               = 0x822F,		// GL_RG16F
	//	
	//	RGB8                = 0x8051,		// GL_RGB8
	//	RGB16               = 0x8054,		// GL_RGB16
	//	RGB16F              = 0x881B,		// GL_RGB16F
	//	
	//	RGBA8               = 0x8058,		// GL_RGBA8
	//	RGBA16              = 0x805B,		// GL_RGBA16
	//	RGBA16F             = 0x881A,		// GL_RGBA16F
	//	
	//	Depth16             = 0x81A5,		// GL_DEPTH_COMPONENT16
	//	Depth24             = 0x81A6,		// GL_DEPTH_COMPONENT24
	//	Depth32             = 0x81A7,		// GL_DEPTH_COMPONENT32
	//	Stencil4            = 0x8D47,		// GL_STENCIL_INDEX4
	//	Stencil8            = 0x8D48,		// GL_STENCIL_INDEX8
	//	Stencil16           = 0x8D49,		// GL_STENCIL_INDEX16
	//	Depth24Stencil8     = 0x88F0		// GL_DEPTH24_STENCIL8
	//};

	enum class AttachmentType : unsigned int {
		Texture = 0,
		RenderBuffer
	};

	struct Attachment {
		unsigned id = 0;
		AttachmentType type = AttachmentType::Texture;
		InternalFormat format = InternalFormat::RGBA8;
		TextureParams parameters = {
			WrapMode::ClampToEdge,
			MinFilter::Nearest,
			MagFilter::Nearest,
			Anisotropy::Aniso1x
		};
		
		Texture2D* texture = nullptr;

		Attachment() = default;
		Attachment(const AttachmentType type, const InternalFormat format) :
			type(type), format(format) {}

		Attachment(const AttachmentType type, const InternalFormat format, const TextureParams parameters) :
			type(type), format(format), parameters(parameters) {}
	};

	#if CAPP_DEBUG || CAPP_RELEASE
	
	inline std::string enumToString(const AttachmentTarget& target) {
		switch(target) {
			case AttachmentTarget::None:			return "None";
			case AttachmentTarget::Colour0:			return "Colour0";
			case AttachmentTarget::Colour1:			return "Colour1";
			case AttachmentTarget::Colour2:			return "Colour2";
			case AttachmentTarget::Colour3:			return "Colour3";
			case AttachmentTarget::Colour4:			return "Colour4";
			case AttachmentTarget::Colour5:			return "Colour5";
			case AttachmentTarget::Colour6:			return "Colour6";
			case AttachmentTarget::Colour7:			return "Colour7";
			case AttachmentTarget::Depth:			return "Depth";
			case AttachmentTarget::Stencil:			return "Stencil";
			case AttachmentTarget::DepthStencil:	return "DepthStencil";
			default:								return "Unknown AttachmentTarget type!";
		}
	}

	inline std::ostream& operator<<(std::ostream& out, const AttachmentTarget& target) { return out << enumToString(target); }
	
	#endif

	
}
