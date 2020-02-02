#pragma once

#include "Cappuccino/Rendering/Texture.h"

#include <glm/glm.hpp>

namespace Capp {

	struct Quad {
		union {
			glm::vec3 zIndexedPosition;
			struct {
				glm::vec2 position;
				float zIndex;
			};
		};
		glm::vec2 dimensions;
		Texture2D* texture;
		glm::vec4 tint;
		float tilingFactor;

		Quad();
		Quad(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec4& colour);
		Quad(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& colour);
		Quad(const glm::vec2& position, float zIndex, const glm::vec2& dimensions, const glm::vec4& colour);
		Quad(const glm::vec3& position, const glm::vec2& dimensions, Texture2D* texture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f);
		Quad(const glm::vec2& position, const glm::vec2& dimensions, Texture2D* texture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f);
		Quad(const glm::vec2& position, float zIndex, const glm::vec2& dimensions, Texture2D* texture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f);
		
	};
	
}