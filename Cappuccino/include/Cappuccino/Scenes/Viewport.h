#pragma once

#include <glm/glm.hpp>

#include <functional>

namespace Capp {
	
	enum class DrawMode : unsigned int {
		Point = 0x1B00,
		Line = 0x1B01,
		Fill = 0x1B02
	};

	struct ViewportProperties {
		using SpecialDrawFunc = std::function<void()>;

		glm::vec4 bounds;
		glm::vec4 borderColour;
		DrawMode drawMode;

		SpecialDrawFunc callback;

		ViewportProperties(
			const glm::vec4&       bounds,
			const glm::vec4&       borderColour,
			DrawMode               drawMode = DrawMode::Fill,
			const SpecialDrawFunc& specialDrawInstructions = []() {}
		);
	};

	class Viewport {

	public:

		Viewport(const ViewportProperties& properties);
		void use() const;

	private:

		ViewportProperties _properties;

	};
	
}