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

	enum class Anisotropy : bool {
		On                   = true,
		Off                  = false
	};
	
	struct TextureParams {
		WrapMode wrapS, wrapT, wrapR;

		MinFilter minFilter;
		MagFilter magFilter;

		Mipmaps enableMipmaps;
		Anisotropy anisotropyEnabled;

		TextureParams() = default;
		TextureParams(WrapMode wrapMode, MinFilter min, MagFilter mag, Mipmaps mipmaps = Mipmaps::On, Anisotropy anisotropy = Anisotropy::On);
		TextureParams(WrapMode s, WrapMode t, WrapMode r, MinFilter min, MagFilter mag, Mipmaps mipmaps = Mipmaps::On, Anisotropy anisotropy = Anisotropy::On);
	};
	
}
