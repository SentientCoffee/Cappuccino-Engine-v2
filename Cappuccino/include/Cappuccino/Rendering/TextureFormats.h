#pragma once

namespace Capp {

	enum class InternalFormat : unsigned int {
		// From glad.h
		None                 = 0,
		R8                   = 0x8229,		// GL_R8
		R16                  = 0x822A,		// GL_R16
		RGB8                 = 0x8051,		// GL_RGB8
		RGB16                = 0x8054,		// GL_RGB16
		RGBA8                = 0x8058,		// GL_RGBA8
		RGBA16               = 0x805B,		// GL_RGBA16
		Depth                = 0x1902,		// GL_DEPTH
		DepthStencil         = 0x84F9		// GL_DEPTH_STENCIL
	};

	enum class PixelFormat : unsigned int {
		// From glad.h
		None                 = 0,
		Red                  = 0x1903,		//  GL_RED
		RG                   = 0x8227,		//  GL_RG
		RGB                  = 0x1907,		//  GL_RGB
		BGR                  = 0x80E0,		//  GL_BGR
		RGBA                 = 0x1908,		//  GL_RGBA
		BGRA                 = 0x80E1,		//  GL_BGRA
		Depth                = 0x1902,		//  GL_DEPTH
		DepthStencil         = 0x84F9		//  GL_DEPTH_STENCIL
	};

	enum class PixelType : unsigned int {
		// From glad.h
		Byte                 = 0x1400,		// GL_BYTE
		UnsignedByte         = 0x1401,		// GL_UNSIGNED_BYTE
		Short                = 0x1402,		// GL_SHORT
		UnsignedShort        = 0x1403,		// GL_UNSIGNED_SHORT
		Int                  = 0x1404,		// GL_INT
		UnsignedInt          = 0x1405,		// GL_UNSIGNED_INT
		Float                = 0x1406		// GL_FLOAT
	};
	
	struct TextureFormats {
		InternalFormat internalFormat = InternalFormat::None;
		PixelFormat pixelFormat = PixelFormat::None;
		PixelType pixelType = PixelType::UnsignedByte;
	};
	
}
