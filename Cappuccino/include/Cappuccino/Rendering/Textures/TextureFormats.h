#pragma once

namespace Capp {

	enum class InternalFormat : unsigned int {
		None                 = 0,
		// From glad.h
		Red8                = 0x8229,		// GL_R8
		Red16               = 0x822A,		// GL_R16
		Red16F              = 0x822D,		// GL_R16F
		Red32F              = 0x822E,		// GL_R32F
		
		RG8                 = 0x822B,		// GL_RG8
		RG16                = 0x822C,		// GL_RG16
		RG16F               = 0x822F,		// GL_RG16F
		RG32F               = 0x8230,		// GL_RG32F
		
		RGB8                = 0x8051,		// GL_RGB8
		RGB16               = 0x8054,		// GL_RGB16
		RGB16F              = 0x881B,		// GL_RGB16F
		RGB32F              = 0x8815,		// GL_RGB32F
		
		RGBA8               = 0x8058,		// GL_RGBA8
		RGBA16              = 0x805B,		// GL_RGBA16
		RGBA16F             = 0x881A,		// GL_RGBA16F
		RGBA32F             = 0x8814,		// GL_RGBA32F
		
		Depth16             = 0x81A5,		// GL_DEPTH_COMPONENT16
		Depth24             = 0x81A6,		// GL_DEPTH_COMPONENT24
		Depth32             = 0x81A7,		// GL_DEPTH_COMPONENT32
		Depth32F            = 0x8CAC,		// GL_DEPTH_COMPONENT32F
		
		Stencil4            = 0x8D47,		// GL_STENCIL_INDEX4
		Stencil8            = 0x8D48,		// GL_STENCIL_INDEX8
		Stencil16           = 0x8D49,		// GL_STENCIL_INDEX16
		
		Depth24Stencil8     = 0x88F0,		// GL_DEPTH24_STENCIL8
	};

	enum class PixelFormat : unsigned int {
		None                 = 0,
		// From glad.h
		Red                  = 0x1903,		//  GL_RED
		RG                   = 0x8227,		//  GL_RG
		RGB                  = 0x1907,		//  GL_RGB
		RGBA                 = 0x1908,		//  GL_RGBA

		BGR                  = 0x80E0,		//  GL_BGR
		BGRA                 = 0x80E1,		//  GL_BGRA
		
		Depth                = 0x1902,		//  GL_DEPTH_COMPONENT
		Stencil              = 0x1901,		//  GL_STENCIL_INDEX
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
		UInt24_UInt8         = 0x84FA,		// GL_UNSIGNED_INT_24_8
		Float                = 0x1406,		// GL_FLOAT
	};
	
	struct TextureFormats {
		InternalFormat internalFormat = InternalFormat::None;
		PixelFormat pixelFormat = PixelFormat::None;
		PixelType pixelType = PixelType::UnsignedByte;
	};
	
}
