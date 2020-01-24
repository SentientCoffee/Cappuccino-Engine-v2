#pragma once

namespace Capp {
	
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
	
}