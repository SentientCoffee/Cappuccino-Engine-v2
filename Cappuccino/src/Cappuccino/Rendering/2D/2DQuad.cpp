#include "CappPCH.h"
#include "Cappuccino/Rendering/2D/2DQuad.h"

using namespace Capp;

ColouredQuad::ColouredQuad() :
	zIndexedPosition(0.0f), dimensions(0.0f), colour(0.0f) {}

ColouredQuad::ColouredQuad(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec4& colour) :
	zIndexedPosition(position), dimensions(dimensions), colour(colour) {}

ColouredQuad::ColouredQuad(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& colour) :
	position(position), zIndex(0.0f), dimensions(dimensions), colour(colour) {}

ColouredQuad::ColouredQuad(const glm::vec2& position, const float zIndex, const glm::vec2& dimensions, const glm::vec4& colour) :
	position(position), zIndex(zIndex), dimensions(dimensions), colour(colour) {}


TexturedQuad::TexturedQuad() :
	zIndexedPosition(0.0f), dimensions(0.0f), texture(nullptr), tint(1.0f), tilingFactor(1.0f) {}

TexturedQuad::TexturedQuad(const glm::vec3& position, const glm::vec2& dimensions, Texture2D* texture, const glm::vec4& tint, const float tilingFactor) :
	zIndexedPosition(position), dimensions(dimensions), texture(texture), tint(tint), tilingFactor(tilingFactor) {}

TexturedQuad::TexturedQuad(const glm::vec2& position, const glm::vec2& dimensions, Texture2D* texture, const glm::vec4& tint, const float tilingFactor) :
	position(position), zIndex(0.0f), dimensions(dimensions), texture(texture), tint(tint), tilingFactor(tilingFactor) {}

TexturedQuad::TexturedQuad(const glm::vec2& position, const float zIndex, const glm::vec2& dimensions, Texture2D* texture, const glm::vec4& tint, const float tilingFactor) :
	position(position), zIndex(zIndex), dimensions(dimensions), texture(texture), tint(tint), tilingFactor(tilingFactor) {}