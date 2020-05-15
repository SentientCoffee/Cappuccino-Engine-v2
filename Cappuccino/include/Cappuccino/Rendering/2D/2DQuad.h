#pragma once

#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Rendering/Textures/Texture2D.h"

#include <glm/glm.hpp>

namespace Capp {

	struct Quad {
		union {
			glm::vec3 zIndexedPosition = { 0.0f, 0.0f, 0.0f };
			struct {
				glm::vec2 position;
				float zIndex;
			};
		};
		glm::vec2 dimensions = { 0.0f, 0.0f };
		Ref<Texture2D> texture = nullptr;
		glm::vec4 tint = { 1.0f, 1.0f, 1.0f, 1.0f };
		float tilingFactor = 1.0f;

		Quad();
		Quad(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec4& colour);
		Quad(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& colour);
		Quad(const glm::vec2& position, float zIndex, const glm::vec2& dimensions, const glm::vec4& colour);
		
		Quad(const glm::vec3& position, const glm::vec2& dimensions, Ref<Texture2D> texture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f);
		Quad(const glm::vec2& position, const glm::vec2& dimensions, Ref<Texture2D> texture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f);
		Quad(const glm::vec2& position, float zIndex, const glm::vec2& dimensions, Ref<Texture2D> texture, const glm::vec4& tint = glm::vec4(1.0f), float tilingFactor = 1.0f);
		
	};
	
}