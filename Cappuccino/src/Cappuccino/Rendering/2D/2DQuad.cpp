#include "CappPCH.h"
#include "Cappuccino/Rendering/2D/2DQuad.h"
#include "Cappuccino/Rendering/Textures/TextureDefaults.h"

using namespace Capp;

Quad::Quad() {}

Quad::Quad(const glm::vec3& position, const glm::vec2& dimensions, const glm::vec4& colour) :
	zIndexedPosition(position), dimensions(dimensions), texture(Texture2D::create(1, 1, &TextureDefaults::whiteTexture)), tint(colour) {}

Quad::Quad(const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& colour) :
	position(position), zIndex(0.0f), dimensions(dimensions), texture(Texture2D::create(1 ,1, &TextureDefaults::whiteTexture)), tint(colour) {}

Quad::Quad(const glm::vec2& position, float zIndex, const glm::vec2& dimensions, const glm::vec4& colour) :
	position(position), zIndex(zIndex), dimensions(dimensions), texture(Texture2D::create(1, 1, &TextureDefaults::whiteTexture)), tint(colour) {}

Quad::Quad(const glm::vec3& position, const glm::vec2& dimensions, Ref<Texture2D> texture, const glm::vec4& tint, float tilingFactor) :
	zIndexedPosition(position), dimensions(dimensions), texture(std::move(texture)), tint(tint), tilingFactor(tilingFactor) {}

Quad::Quad(const glm::vec2& position, const glm::vec2& dimensions, Ref<Texture2D> texture, const glm::vec4& tint, float tilingFactor) :
	position(position), zIndex(0.0f), dimensions(dimensions), texture(std::move(texture)), tint(tint), tilingFactor(tilingFactor) {}

Quad::Quad(const glm::vec2& position, float zIndex, const glm::vec2& dimensions, Ref<Texture2D> texture, const glm::vec4& tint, float tilingFactor) :
	position(position), zIndex(zIndex), dimensions(dimensions), texture(std::move(texture)), tint(tint), tilingFactor(tilingFactor) {}
