#pragma once

#include "Cappuccino/Input/KeyCodes.h"
#include "Cappuccino/Input/MouseButtonCodes.h"

#include <glm/glm.hpp>

namespace Capp {
	
	class Input {
	public:

		static bool keyPressed(KeyCode key);
		static bool keyReleased(KeyCode key);

		static bool mouseButtonPressed(MouseButton button);
		static bool mouseButtonReleased(MouseButton button);

		static glm::vec2 getMousePosition();

	};
}