#include "CappPCH.h"
#include "Cappuccino/Rendering/2D/Renderer2D.h"

#include "Cappuccino/Rendering/RenderCommand.h"
#include "Cappuccino/Rendering/Shader.h"
#include "Cappuccino/Rendering/3D/Mesh.h"
#include "Cappuccino/Objects/Transform.h"

using namespace Capp;

struct Renderer2DStorage {

	Mesh* quadMesh = nullptr;
	Shader* quadShader = nullptr;
	Shader* textShader = nullptr;
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
	renderer2DStorage->textShader = new Shader("DefaultText", "Assets/Cappuccino/Shaders/TextShader.vert", "Assets/Cappuccino/Shaders/TextShader.frag");

	renderer2DStorage->quadShader->bind();
	renderer2DStorage->quadShader->setUniform("uTextureSlot", 0);

	//renderer2DStorage->textShader->bind();
	//renderer2DStorage->textShader->setUniform("uTextureSlot", 0);

	unsigned whiteTextureData = 0xFFFFFFFF;
	renderer2DStorage->whiteTexture = new Texture2D(1, 1, &whiteTextureData);
}

void Renderer2D::shutdown() {
	delete renderer2DStorage;
}

void Renderer2D::start(const OrthographicCamera& camera) {
	renderer2DStorage->quadShader->bind();
	renderer2DStorage->quadShader->setUniform("uViewProjection", camera.getViewProjection());

	renderer2DStorage->textShader->bind();
	renderer2DStorage->textShader->setUniform("uProjection", camera.getProjectionMatrix());
}

void Renderer2D::finish() {}

void Renderer2D::drawQuad(const ColouredQuad& properties) {
	renderer2DStorage->whiteTexture->bind(0);

	Transform transform;
	transform.setPosition(properties.zIndexedPosition).setScale(properties.dimensions.x, properties.dimensions.y, 0.0f);

	renderer2DStorage->quadShader->bind();
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

	renderer2DStorage->quadShader->bind();
	renderer2DStorage->quadShader->setUniform("uTransform", transform.getWorldTransform());
	renderer2DStorage->quadShader->setUniform("uColour", properties.tint);
	renderer2DStorage->quadShader->setUniform("uTileFactor", properties.tilingFactor);

	renderer2DStorage->quadMesh->getVAO()->bind();
	RenderCommand::drawIndexed(renderer2DStorage->quadMesh->getVAO());
}

//void Renderer2D::drawText(Text* text) {
//	renderer2DStorage->textShader->bind();
//	//renderer2DStorage->textShader->setUniform("uTextColour", text->getTextColour());
//
//	auto tempPos = text->getTransform().getPosition();
//	for(auto ch = text->getText().begin(); ch != text->getText().end(); ++ch) {
//		const Glyph glyph = text->getFont()->getCharacter(*ch);
//		
//		const float x = tempPos.x + glyph.bearing.x * text->getTransform().getScale().x;
//		const float y = tempPos.y - (glyph.size.y - glyph.bearing.y) * text->getTransform().getScale().y;
//		const float w = glyph.size.x * text->getTransform().getScale().x;
//		const float h = glyph.size.y * text->getTransform().getScale().y;
//
//		std::vector<float> vertices = {
//			x,     y + h,   0.0, 0.0,
//			x,     y,       0.0, 1.0,
//			x + w, y,       1.0, 1.0,
//
//			x,     y + h,   0.0, 0.0,
//			x + w, y,       1.0, 1.0,
//			x + w, y + h,   1.0, 0.0
//		};
//
//		glyph.texture->bind(0);
//		text->getVAO()->getVertexBuffers().at(0)->setBufferData(vertices);
//
//		RenderCommand::drawArray(text->getVAO());
//		// Advance cursors for next glyph (note that advance is number of 1/64 pixels)
//		tempPos.x += (glyph.advance >> 6)* text->getTransform().getScale().x;  // Bit shift by 6 to get value in pixels (2^6 = 64)
//	}
//}