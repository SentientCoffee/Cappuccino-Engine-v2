#pragma once

namespace Capp {

	enum class WrapMode : unsigned int {
		// From glad.h
		ClampToEdge          = 0x812F,
		ClampToBorder        = 0x812D,
		MirroredRepeat       = 0x8370,
		Repeat               = 0x2901,
		MirrorClampToEdge    = 0x8743
	};

	enum class MinFilter : unsigned int {
		// From glad.h
		Nearest              = 0x2600,
		Linear               = 0x2601,
		NearestMipmapNearest = 0x2700,
		LinearMipmapNearest  = 0x2701,
		NearestMipmapLinear  = 0x2702,
		LinearMipmapLinear   = 0x2703
	};

	enum class MagFilter : unsigned int {
		// From glad.h
		Nearest              = 0x2600,
		Linear               = 0x2601
	};

	enum class Mipmaps : bool {
		On                   = true,
		Off                  = false
	};

	enum class Anisotropy : bool {
		On                   = true,
		Off                  = false
	};
	
}
