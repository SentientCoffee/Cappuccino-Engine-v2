#include "CappPCH.h"
#include "Cappuccino/Rendering/2D/Renderer2D.h"

#include "Cappuccino/Core/Application.h"
#include "Cappuccino/Objects/Transform.h"

#include "Cappuccino/Rendering/RenderCommand.h"
#include "Cappuccino/Rendering/3D/Mesh.h"
#include "Cappuccino/Rendering/Buffers/Framebuffer.h"
#include "Cappuccino/Rendering/Shaders/Shader.h"
#include "Cappuccino/Rendering/Textures/TextureDefaults.h"

using namespace Capp;

struct Renderer2DStorage {
	OrthographicCamera defaultCamera;
	Mesh* quadMesh = nullptr;
	Shader* quadShader = nullptr;
	Shader* textShader = nullptr;
	Texture2D* whiteTexture = nullptr;

	Framebuffer* mainBuffer = nullptr;
	Shader* mainBufferShader = nullptr;
	Mesh* fullscreenQuad = nullptr;
	
	std::vector<Quad> quadsToRender;
	std::vector<Text*> textToRender;
};

static Renderer2DStorage* renderer2DStorage;

void Renderer2D::init() {
	
	renderer2DStorage = new Renderer2DStorage;
	const auto window = Application::getInstance()->getWindow();

	{
		renderer2DStorage->defaultCamera.setProjection(0, static_cast<float>(window->getWidth()), 0, static_cast<float>(window->getHeight()));
	}
	
	{
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
	
		renderer2DStorage->quadMesh = new Mesh("DefaultQuad", quadVertices, quadIndices);

		const std::vector<Vertex> vertices = {
			{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
			{ {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
			{ {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f } },
			{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } }
		};

		const std::vector<unsigned> indices = {
			0, 1, 2,
			0, 2, 3
		};

		renderer2DStorage->fullscreenQuad = new Mesh("DefaultFramebufferFullscreenQuad", vertices, indices);
	}

	{
		renderer2DStorage->quadShader = new Shader("2D Default");
		renderer2DStorage->quadShader->attach("Assets/Cappuccino/Shaders/2DShader.vert", ShaderStage::Vertex);
		renderer2DStorage->quadShader->attach("Assets/Cappuccino/Shaders/2DShader.frag", ShaderStage::Fragment);
		renderer2DStorage->quadShader->compile();
		renderer2DStorage->quadShader->bind();
		renderer2DStorage->quadShader->setUniform<Int>("uTextureSlot", 0);

		renderer2DStorage->textShader = new Shader("Text Default");
		renderer2DStorage->textShader->attach("Assets/Cappuccino/Shaders/TextShader.vert", ShaderStage::Vertex);
		renderer2DStorage->textShader->attach("Assets/Cappuccino/Shaders/TextShader.frag", ShaderStage::Fragment);
		renderer2DStorage->textShader->compile();
		renderer2DStorage->textShader->bind();
		renderer2DStorage->textShader->setUniform<Int>("uTextureSlot", 0);

		renderer2DStorage->mainBufferShader = new Shader("2D Framebuffer Default");
		renderer2DStorage->mainBufferShader->attach("Assets/Cappuccino/Shaders/FramebufferShader.vert", ShaderStage::Vertex);
		renderer2DStorage->mainBufferShader->attach("Assets/Cappuccino/Shaders/FramebufferShader.frag", ShaderStage::Fragment);
		renderer2DStorage->mainBufferShader->compile();
		renderer2DStorage->mainBufferShader->bind();
		renderer2DStorage->mainBufferShader->setUniform<Int>("uTextureSlot", 0);
	}

	{
		renderer2DStorage->whiteTexture = new Texture2D(1, 1, &whiteTexture);
	}

	{
		renderer2DStorage->mainBuffer = new Framebuffer(window->getWidth(), window->getHeight());
		renderer2DStorage->mainBuffer->setName("Main 2D Framebuffer");
		const Attachment mainColour = { AttachmentType::Texture, InternalFormat::RGBA8 };
		const Attachment depthStencil = { AttachmentType::RenderBuffer, InternalFormat::Depth24_Stencil8 };
		renderer2DStorage->mainBuffer->addAttachment(AttachmentTarget::Colour0, mainColour);
		renderer2DStorage->mainBuffer->addAttachment(AttachmentTarget::DepthStencil, depthStencil);
	}
	
}

void Renderer2D::shutdown() {
	delete renderer2DStorage;
}

void Renderer2D::onWindowResized(const unsigned width, const unsigned height) {
	renderer2DStorage->defaultCamera.setProjection(0, static_cast<float>(width), 0, static_cast<float>(height));
	renderer2DStorage->mainBuffer->resize(width, height);
}

void Renderer2D::start() {
	start(renderer2DStorage->defaultCamera);
}

void Renderer2D::start(const OrthographicCamera& camera) {
	RenderCommand::disableCulling();
	
	renderer2DStorage->quadShader->bind();
	renderer2DStorage->quadShader->setUniform<Mat4>("uViewProjection", camera.getViewProjection());

	renderer2DStorage->textShader->bind();
	renderer2DStorage->textShader->setUniform<Mat4>("uProjection", camera.getProjectionMatrix());

	renderer2DStorage->quadsToRender.clear();
	renderer2DStorage->quadsToRender.reserve(200);
	renderer2DStorage->textToRender.clear();
	renderer2DStorage->textToRender.reserve(200);
}

void Renderer2D::drawQuad(const Quad& quad) {
	CAPP_ASSERT(quad.texture != nullptr, "No texture in textured quad!");
	renderer2DStorage->quadsToRender.push_back(quad);
}

void Renderer2D::drawText(Text* text) {
	renderer2DStorage->textToRender.push_back(text);
}

void Renderer2D::finish() {

	renderer2DStorage->mainBuffer->bind();
	RenderCommand::setViewport(0, 0, renderer2DStorage->mainBuffer->getWidth(), renderer2DStorage->mainBuffer->getHeight());
	// CLEAR WITH ALPHA AT 0.0f SO OTHER LAYERS' FRAMEBUFFERS CAN BE SEEN
	RenderCommand::setClearColour(0.0f, 0.0f, 0.0f, 0.0f);
	RenderCommand::clearScreen();
	RenderCommand::enableDepthTesting();
	RenderCommand::setSeparateBlendFunction(SourceFactor::SourceAlpha, DestinationFactor::OneMinusSourceAlpha, SourceFactor::One, DestinationFactor::OneMinusSourceAlpha);
	
	renderer2DStorage->quadShader->bind();
	Transform transform;

	std::sort(renderer2DStorage->quadsToRender.begin(), renderer2DStorage->quadsToRender.end(), [](const Quad& first, const Quad& second)->bool {
		return first.zIndex < second.zIndex;
	});
	
	for(auto quad : renderer2DStorage->quadsToRender) {
		CAPP_ASSERT(quad.texture != nullptr, "No texture in textured quad!");
		quad.texture->bind(0);

		transform.setPosition(quad.zIndexedPosition).setScale(quad.dimensions.x, quad.dimensions.y, 0.0f);

		renderer2DStorage->quadShader->setUniform<Mat4>("uTransform", transform.getWorldTransform());
		renderer2DStorage->quadShader->setUniform<Vec4>("uColour", quad.tint);
		renderer2DStorage->quadShader->setUniform<Float>("uTileFactor", quad.tilingFactor);

		renderer2DStorage->quadMesh->getVAO()->bind();
		RenderCommand::drawIndexed(renderer2DStorage->quadMesh->getVAO());
	}

	renderer2DStorage->textShader->bind();
	for(auto text : renderer2DStorage->textToRender) {
		renderer2DStorage->textShader->setUniform<Vec4>("uTextColour", text->getTextColour());

		auto tempPos = text->getTransform().getPosition();
		for(auto ch = text->getText().begin(); ch != text->getText().end(); ++ch) {
			const Glyph glyph = text->getFont()->getCharacter(*ch);

			const float x = tempPos.x + glyph.bearing.x * text->getTransform().getScale().x;
			const float y = tempPos.y + static_cast<float>(glyph.size.y - glyph.bearing.y)* text->getTransform().getScale().y;
			const float w = glyph.size.x * text->getTransform().getScale().x;
			const float h = glyph.size.y * text->getTransform().getScale().y;

			std::vector<float> vertices = {
				x,     y,       0.0f, 1.0f,
				x + w, y,       1.0f, 1.0f,
				x + w, y - h,   1.0f, 0.0f,
				x,     y - h,   0.0f, 0.0f,
			};

			std::vector<unsigned> indices = {
				0, 1, 2,
				0, 2, 3
			};

			glyph.texture->bind(0);
			text->getVAO()->bind();
			text->getVAO()->getVertexBuffers().at(0)->setBufferData(vertices);
			text->getVAO()->getIndexBuffer()->setBufferData(indices);

			RenderCommand::drawIndexed(text->getVAO());
			// Advance cursors for next glyph (note that advance is number of 1/64 pixels)
			tempPos.x += static_cast<float>(glyph.advance >> 6)* text->getTransform().getScale().x;  // Bit shift by 6 to get value in pixels (2^6 = 64)
		}
	}

	renderer2DStorage->mainBuffer->unbind();
	RenderCommand::disableDepthTesting();
	RenderCommand::setBlendFunction(SourceFactor::SourceAlpha, DestinationFactor::OneMinusSourceAlpha);
	
	RenderCommand::setViewport(0, 0, renderer2DStorage->mainBuffer->getWidth(), renderer2DStorage->mainBuffer->getHeight());
	renderer2DStorage->mainBufferShader->bind();
	renderer2DStorage->mainBuffer->getAttachment(AttachmentTarget::Colour0)->bind(0);
	renderer2DStorage->fullscreenQuad->getVAO()->bind();
	RenderCommand::drawIndexed(renderer2DStorage->fullscreenQuad->getVAO());

	
	for(unsigned i = 0; i < 24; ++i) {
		Texture2D::unbind(i);
	}
	Shader::unbind();
	VertexArray::unbind();
}