#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <functional>

struct ViewportProperties {
	using SpecialDrawFunc = std::function<void()>;

	glm::vec4 bounds;
	glm::vec4 borderColour;
	GLenum drawMode;

	SpecialDrawFunc callback;

	ViewportProperties(
		const glm::vec4& bounds,
		const glm::vec4& borderColour,
		GLenum           drawMode = GL_FILL,
		SpecialDrawFunc  specialDrawInstructions = []() {}
	);
};

class Viewport {

public:

	Viewport(const ViewportProperties& properties);
	void use() const;

private:

	ViewportProperties _properties;

};