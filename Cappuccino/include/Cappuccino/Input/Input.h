#pragma once

#include <glm/glm.hpp>

namespace Capp {
	
	class Input {
	public:

		static bool keyPressed(uint32_t key);
		static bool keyReleased(uint32_t key);

		static bool mouseButtonPressed(uint32_t button);
		static bool mouseButtonReleased(uint32_t button);

		static glm::vec2 getMousePosition();

	};
}