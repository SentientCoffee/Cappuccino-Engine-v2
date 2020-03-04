#pragma once

namespace Capp {

	enum class WrapMode : unsigned int {
		// From glad.h
		ClampToEdge          = 0x812F,		// GL_CLAMP_TO_EDGE
		ClampToBorder        = 0x812D,		// GL_CLAMP_TO_BORDER
		MirroredRepeat       = 0x8370,		// GL_MIRRORED_REPEAT
		Repeat               = 0x2901,		// GL_REPEAT
		MirrorClampToEdge    = 0x8743		// GL_MIRROR_CLAMP_TO_EDGE
	};

	enum class MinFilter : unsigned int {
		// From glad.h
		Nearest              = 0x2600,		// GL_NEAREST
		Linear               = 0x2601,		// GL_LINEAR
		NearestMipmapNearest = 0x2700,		// GL_NEAREST_MIPMAP_NEAREST
		LinearMipmapNearest  = 0x2701,		// GL_LINEAR_MIPMAP_NEAREST
		NearestMipmapLinear  = 0x2702,		// GL_NEAREST_MIPMAP_LINEAR
		LinearMipmapLinear   = 0x2703		// GL_LINEAR_MIPMAP_LINEAR
	};

	enum class MagFilter : unsigned int {
		// From glad.h
		Nearest              = 0x2600,		// GL_NEAREST
		Linear               = 0x2601		// GL_LINEAR
	};

	enum class Mipmaps : bool {
		On                   = true,
		Off                  = false
	};

	enum class Anisotropy : unsigned int {
		Aniso1x              = 1,
		Aniso2x              = 2,
		Aniso4x              = 4,
		Aniso8x              = 8,
		Aniso16x             = 16,
	};
	
	struct TextureParams {
		WrapMode wrapS = WrapMode::Repeat;
		WrapMode wrapT = WrapMode::Repeat;
		WrapMode wrapR = WrapMode::Repeat;

		MinFilter minFilter = MinFilter::Linear;
		MagFilter magFilter = MagFilter::Nearest;
		
		Anisotropy anisotropyLevel = Anisotropy::Aniso1x;
		glm::vec4 borderColour = { 0.0f, 0.0f, 0.0f, 1.0f };

		TextureParams() = default;
		TextureParams(WrapMode wrapMode, MinFilter minFilter, MagFilter magFilter, Anisotropy anisotropyLevel = Anisotropy::Aniso1x, const glm::vec4& borderColour = { 0.0f, 0.0f, 0.0f, 1.0f });
		TextureParams(WrapMode wrapS, WrapMode wrapT, WrapMode wrapR, MinFilter minFilter, MagFilter magFilter, Anisotropy anisotropyLevel = Anisotropy::Aniso1x, const glm::vec4& borderColour = { 0.0f, 0.0f, 0.0f, 1.0f });
	};
	
}