#include "CappPCH.h"
#include "Cappuccino/Rendering/2D/2DQuad.h"
#include "Cappuccino/Rendering/TextureDefaults.h"

using namespace Capp;

Quad::Quad() :
	zIndexedPosition(0.0f, 0.0f, 0.0f), dimensions(0.0f, 0.0f), texture(nullptr), tint(0.0f, 0.0f, 0.0f, 0.0f), tilingFactor(1.0f) {}

Quad::Quad(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec4& colour) :
	zIndexedPosition(position), dimensions(dimensions), texture(new Texture2D(1, 1, &whiteTexture)), tint(colour), tilingFactor(1.0f) {}

Quad::Quad(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& colour) :
	position(position), zIndex(0.0f), dimensions(dimensions), texture(new Texture2D(1 ,1, &whiteTexture)), tint(colour), tilingFactor(1.0f) {}

Quad::Quad(const glm::vec2& position, float zIndex, const glm::vec2& dimensions, const glm::vec4& colour) :
	position(position), zIndex(zIndex), dimensions(dimensions), texture(new Texture2D(1, 1, &whiteTexture)), tint(colour), tilingFactor(1.0f) {}

Quad::Quad(const glm::vec3& position, const glm::vec2& dimensions, Texture2D* texture, const glm::vec4& tint, float tilingFactor) :
	zIndexedPosition(position), dimensions(dimensions), texture(texture), tint(tint), tilingFactor(tilingFactor) {}

Quad::Quad(const glm::vec2& position, const glm::vec2& dimensions, Texture2D* texture, const glm::vec4& tint, float tilingFactor) :
	position(position), zIndex(0.0f), dimensions(dimensions), texture(texture), tint(tint), tilingFactor(tilingFactor) {}

Quad::Quad(const glm::vec2& position, float zIndex, const glm::vec2& dimensions, Texture2D* texture, const glm::vec4& tint, float tilingFactor) :
	position(position), zIndex(zIndex), dimensions(dimensions), texture(texture), tint(tint), tilingFactor(tilingFactor) {}
