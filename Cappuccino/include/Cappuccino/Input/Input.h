#pragma once

#include <glm/glm.hpp>

namespace Capp {
	
	class Input {
	public:

		static bool keyPressed(unsigned int key);
		static bool keyReleased(unsigned int key);

		static bool mouseButtonPressed(unsigned int button);
		static bool mouseButtonReleased(unsigned int button);

		static glm::vec2 getMousePosition();

	};
}