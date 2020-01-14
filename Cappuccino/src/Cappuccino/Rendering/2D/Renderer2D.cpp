#include "CappPCH.h"
#include "Cappuccino/Rendering/2D/Renderer2D.h"

#include "Cappuccino/Rendering/RenderCommand.h"
#include "Cappuccino/Rendering/Shader.h"
#include "Cappuccino/Rendering/3D/Mesh.h"
#include "Cappuccino/Objects/Transform.h"
#include "Cappuccino/Rendering/3D/Renderer.h"

using namespace Capp;

struct Renderer2DStorage {

	Mesh* quadMesh = nullptr;
	Shader* quadShader = nullptr;
	Texture2D* whiteTexture = nullptr;

};

static Renderer2DStorage* renderer2DStorage;

void Renderer2D::init() {

	renderer2DStorage = new Renderer2DStorage;

	const std::vector<Vertex> quadVertices = {
		{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
		{ {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
		{ {  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
		{ { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } }
	};

	const std::vector<unsigned> quadIndices = {
		0, 1, 2,
		0, 2, 3
	};
	
	renderer2DStorage->quadMesh = new Mesh(quadVertices, quadIndices);

	renderer2DStorage->quadShader = new Shader("Default2D", "Assets/Cappuccino/Shaders/2DShader.vert", "Assets/Cappuccino/Shaders/2DShader.frag");

	renderer2DStorage->quadShader->bind();
	renderer2DStorage->quadShader->setUniform("uTextureSlot", 0);

	unsigned whiteTextureData = 0xFFFFFFFF;
	renderer2DStorage->whiteTexture = new Texture2D(1, 1, &whiteTextureData);
	renderer2DStorage->whiteTexture->setData(&whiteTextureData, sizeof(unsigned));
}

void Renderer2D::shutdown() {
	delete renderer2DStorage;
}

void Renderer2D::start(const OrthographicCamera& camera) {
	renderer2DStorage->quadShader->bind();
	renderer2DStorage->quadShader->setUniform("uViewProjection", camera.getViewProjection());
}

void Renderer2D::finish() {}

void Renderer2D::drawQuad(const ColouredQuad& properties) {
	renderer2DStorage->whiteTexture->bind(0);

	Transform transform;
	transform.setPosition(properties.zIndexedPosition).setScale(properties.dimensions.x, properties.dimensions.y, 0.0f);
	//const glm::mat4 transform = glm::translate(glm::mat4(1.0f), properties.zIndexedPosition) *
	//		glm::scale(glm::mat4(1.0f), glm::vec3(properties.dimensions.x, properties.dimensions.y, 0.0f));
	
	renderer2DStorage->quadShader->setUniform("uTransform", transform.getWorldTransform());
	renderer2DStorage->quadShader->setUniform("uColour", properties.colour);
	renderer2DStorage->quadShader->setUniform("uTileFactor", 1.0f);

	renderer2DStorage->quadMesh->getVAO()->bind();
	RenderCommand::drawIndexed(renderer2DStorage->quadMesh->getVAO());

	Texture2D::unbind(0);
}

void Renderer2D::drawQuad(const TexturedQuad& properties) {
	CAPP_ASSERT(properties.texture != nullptr, "No texture in textured quad!");
	properties.texture->bind(0);

	Transform transform;
	transform.setPosition(properties.zIndexedPosition).setScale(properties.dimensions.x, properties.dimensions.y, 0.0f);
	//const glm::mat4 transform = glm::translate(glm::mat4(1.0f), properties.zIndexedPosition) *
	//		glm::scale(glm::mat4(1.0f), glm::vec3(properties.dimensions.x, properties.dimensions.y, 0.0f));
	renderer2DStorage->quadShader->setUniform("uTransform", transform.getWorldTransform());
	renderer2DStorage->quadShader->setUniform("uColour", properties.tint);
	renderer2DStorage->quadShader->setUniform("uTileFactor", properties.tilingFactor);

	renderer2DStorage->quadMesh->getVAO()->bind();
	RenderCommand::drawIndexed(renderer2DStorage->quadMesh->getVAO());
}
