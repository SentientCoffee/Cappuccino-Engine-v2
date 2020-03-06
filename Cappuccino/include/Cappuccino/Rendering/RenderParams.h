#pragma once

namespace Capp {

	enum class ClearFlags : unsigned int {
		// From glad.h
		Colour              = 0x00004000,					// GL_COLOR_BUFFER_BIT
		Depth               = 0x00000100,					// GL_DEPTH_BUFFER_BIT
		Stencil             = 0x00000400,					// GL_STENCIL_BUFFER_BIT
		NoColor             = Depth | Stencil,				// GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
		NoDepth             = Colour | Stencil,				// GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
		NoStencil           = Colour | Depth,				// GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
		All                 = Colour | Depth | Stencil		// GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
	};

	enum class CullMode : unsigned int {
		FrontFace           = 0x0404,		// GL_FRONT
		BackFace            = 0x0405,		// GL_BACK
		Both                = 0x0408		// GL_FRONT_AND_BACK
	};
	
	enum class DrawMode : unsigned int {
		// From glad.h
		Point               = 0x1B00,		// GL_POINT
		Line                = 0x1B01,		// GL_LINE
		Fill                = 0x1B02		// GL_FILL
	};

	enum class DepthTestFunction : unsigned int {
		// From glad.h
		Never               = 0x0200,		// GL_NEVER
		NotEqual            = 0x0205,		// GL_NOTEQUAL
		LessThan            = 0x0201,		// GL_LESS
		LessThanOrEqual     = 0x0203,		// GL_LEQUAL
		Equal               = 0x0202,		// GL_EQUAL
		GreaterThanOrEqual  = 0x0206,		// GL_GEQUAL
		GreaterThan         = 0x0204,		// GL_GREATER
		Always              = 0x0207		// GL_ALWAYS
	};

	enum class StencilTestFunction : unsigned int {
		// From glad.h
		Never               = 0x0200,		// GL_NEVER
		NotEqual            = 0x0205,		// GL_NOTEQUAL
		LessThan            = 0x0201,		// GL_LESS
		LessThanOrEqual     = 0x0203,		// GL_LEQUAL
		Equal               = 0x0202,		// GL_EQUAL
		GreaterThanOrEqual  = 0x0206,		// GL_GEQUAL
		GreaterThan         = 0x0204,		// GL_GREATER
		Always              = 0x0207		// GL_ALWAYS
	};

	enum class StencilOperation : unsigned int {
		// From glad.h
		Zero                = 0x0000,		// GL_ZERO
		Keep                = 0x1E00,		// GL_KEEP
		Replace             = 0x1E01,		// GL REPLACE
		Increment           = 0x1E02,		// GL_INCR
		Decrement           = 0x1E03,		// GL_DECR
		IncrementWrap       = 0x8507,		// GL_INCR_WRAP
		DecrementWrap       = 0x8508,		// GL_DECR_WRAP
		Invert              = 0x150A		// GL_INVERT
	};

	enum class SourceFactor : unsigned int {
		// From glad.h
		Zero                      = 0x0000,		// GL_ZERO
		One                       = 0x0001,		// GL_ONE
		SourceColour              = 0x0300,		// GL_SRC_COLOR
		OneMinusSourceColour      = 0x0301,		// GL_ONE_MINUS_SRC_COLOR
		SourceAlpha               = 0x0302,		// GL_SRC_ALPHA
		OneMinusSourceAlpha       = 0x0303,		// GL_ONE_MINUS_SRC_ALPHA
		DestinationAlpha          = 0x0304,		// GL_DST_ALPHA
		OneMinusDestinationAlpha  = 0x0305,		// GL_ONE_MINUS_DST_ALPHA
		DestinationColour         = 0x0306,		// GL_DST_COLOR
		OneMinusDestinationColour = 0x0307,		// GL_ONE_MINUS_DST_COLOR
		SourceAlphaSaturate       = 0x0308,		// GL_SRC_ALPHA_SATURATE
		ConstantColour            = 0x8001,		// GL_CONSTANT_COLOR
		OneMinusConstantColour    = 0x8002,		// GL_ONE_MINUS_CONSTANT_COLOR
		ConstantAlpha             = 0x8003,		// GL_CONSTANT_ALPHA
		OneMinusConstantAlpha     = 0x8004,		// GL_ONE_MINUS_CONSTANT_ALPHA
	};
	
	enum class DestinationFactor : unsigned int {
		// From glad.h
		Zero                      = 0x0000,		// GL_ZERO
		One                       = 0x0001,		// GL_ONE
		SourceColour              = 0x0300,		// GL_SRC_COLOR
		OneMinusSourceColour      = 0x0301,		// GL_ONE_MINUS_SRC_COLOR
		SourceAlpha               = 0x0302,		// GL_SRC_ALPHA
		OneMinusSourceAlpha       = 0x0303,		// GL_ONE_MINUS_SRC_ALPHA
		DestinationAlpha          = 0x0304,		// GL_DST_ALPHA
		OneMinusDestinationAlpha  = 0x0305,		// GL_ONE_MINUS_DST_ALPHA
		DestinationColour         = 0x0306,		// GL_DST_COLOR
		OneMinusDestinationColour = 0x0307,		// GL_ONE_MINUS_DST_COLOR
		ConstantColour            = 0x8001,		// GL_CONSTANT_COLOR
		OneMinusConstantColour    = 0x8002,		// GL_ONE_MINUS_CONSTANT_COLOR
		ConstantAlpha             = 0x8003,		// GL_CONSTANT_ALPHA
		OneMinusConstantAlpha     = 0x8004		// GL_ONE_MINUS_CONSTANT_ALPHA
	};
}
