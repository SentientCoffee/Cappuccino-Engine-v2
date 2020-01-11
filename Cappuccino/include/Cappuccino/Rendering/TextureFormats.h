#pragma once

namespace Capp {

	enum class InternalFormat : unsigned int {
		// From glad.h
		None                 = 0,
		R8                   = 0x8229,
		R16                  = 0x822A,
		RGB8                 = 0x8051,
		RGB16                = 0x8054,
		RGBA8                = 0x8058,
		RGBA16               = 0x805B,
		Depth                = 0x1902,
		DepthStencil         = 0x84F9
	};

	enum class PixelFormat : unsigned int {
		// From glad.h
		None                 = 0,
		Red                  = 0x1903,
		RG                   = 0x8227,
		RGB                  = 0x1907,
		BGR                  = 0x80E0,
		RGBA                 = 0x1908,
		BGRA                 = 0x80E1,
		Depth                = 0x1902,
		DepthStencil         = 0x84F9
	};

	enum class PixelType : unsigned int {
		// From glad.h
		UnsignedByte         = 0x1401,
		Byte                 = 0x1400,
		UnsignedShort        = 0x1403,
		Short                = 0x1402,
		UnsignedInt          = 0x1405,
		Int                  = 0x1404,
		Float                = 0x1406
	};
	
}
