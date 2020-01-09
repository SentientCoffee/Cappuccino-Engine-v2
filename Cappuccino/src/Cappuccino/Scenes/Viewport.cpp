#include "CappPCH.h"
#include "Cappuccino/Scenes/Viewport.h"

ViewportProperties::ViewportProperties(const glm::vec4& bounds, const glm::vec4& borderColour, const GLenum drawMode, const SpecialDrawFunc specialDrawInstructions) :
	bounds(bounds), borderColour(borderColour), drawMode(drawMode), callback(specialDrawInstructions) {}

Viewport::Viewport(const ViewportProperties& properties) :
	_properties(properties) {}

void Viewport::use() const {
	glViewport(static_cast<int>(_properties.bounds.x), static_cast<int>(_properties.bounds.y), static_cast<int>(_properties.bounds.z), static_cast<int>(_properties.bounds.w));
	glScissor(static_cast<int>(_properties.bounds.x), static_cast<int>(_properties.bounds.y), static_cast<int>(_properties.bounds.z), static_cast<int>(_properties.bounds.w));

	glClearColor(_properties.borderColour.r, _properties.borderColour.g, _properties.borderColour.b, _properties.borderColour.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, _properties.drawMode);
	_properties.callback != nullptr ? _properties.callback() : 0;
}