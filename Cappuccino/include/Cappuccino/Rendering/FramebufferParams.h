#pragma once

namespace Capp {
	
	enum class FramebufferBinding : unsigned int {
		None = 0,
		// From glad.h
		DrawOnly = 0x8CA9,		// GL_DRAW_FRAMEBUFFER
		WriteOnly = 0x8CA9,		// GL_DRAW_FRAMEBUFFER
		ReadOnly = 0x8CA8,		// GL_READ_FRAMEBUFFER
		ReadWrite = 0x8D40		// GL_FRAMEBUFFER
	};
	
}