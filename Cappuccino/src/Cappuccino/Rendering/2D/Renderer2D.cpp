#include "CappPCH.h"
#include "Cappuccino/Rendering/2D/Renderer2D.h"

#include "Cappuccino/Core/Application.h"
#include "Cappuccino/Objects/Transform.h"

#include "Cappuccino/Rendering/RenderCommand.h"
#include "Cappuccino/Rendering/Mesh.h"
#include "Cappuccino/Rendering/Buffers/Framebuffer.h"
#include "Cappuccino/Rendering/Shaders/Shader.h"
#include "Cappuccino/Rendering/Textures/TextureDefaults.h"

using namespace Capp;

struct Renderer2DStorage {
	OrthographicCamera defaultCamera;
	
	Ref<Framebuffer> mainBuffer = nullptr;

	Ref<Mesh> quadMesh = nullptr;
	Ref<Mesh> fullscreenQuad = nullptr;

	Ref<Shader> quadShader = nullptr;
	Ref<Shader> textShader = nullptr;
	Ref<Shader> mainBufferShader = nullptr;

	Ref<Texture2D> whiteTexture = nullptr;
	
	std::deque<Quad> quadQueue;
	std::deque<Ref<Text>> textQueue;
};

static Renderer2DStorage r2DStorage;

void Renderer2D::init() {
	
	const auto window = Application::getInstance()->getWindow();

	// --------------------------------------------------
	// ----- Camera init --------------------------------
	// --------------------------------------------------
	
	{
		r2DStorage.defaultCamera.setProjection(0, static_cast<float>(window->getWidth()), 0, static_cast<float>(window->getHeight()));
	}

	// --------------------------------------------------
	// ----- Meshes init --------------------------------
	// --------------------------------------------------

	{
		// Renderable quad
		const std::array<Vertex, 4> quadVertices = { {
			{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
			{ {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
			{ {  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
			{ { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } }
		} };

		const std::array<uint32_t, 6> quadIndices = {
			0, 1, 2,
			0, 2, 3
		};
	
		r2DStorage.quadMesh = Mesh::create("DefaultQuad", quadVertices, quadIndices);

		// Fullscreen quad
		const std::array<Vertex, 4> vertices = { {
			{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
			{ {  1.0f, -1.0f, 0.0f }, { 1.0f, 0.0f } },
			{ {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f } },
			{ { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f } }
		} };

		const std::array<uint32_t, 6> indices = {
			0, 1, 2,
			0, 2, 3
		};

		r2DStorage.fullscreenQuad = Mesh::create("DefaultFramebufferFullscreenQuad", vertices, indices);
	}

	// --------------------------------------------------
	// ----- Shaders init -------------------------------
	// --------------------------------------------------
	
	{
		r2DStorage.quadShader = Shader::create("2D Default");
		r2DStorage.quadShader->attach("Assets/Cappuccino/Shaders/2DShader.vert", ShaderStage::Vertex);
		r2DStorage.quadShader->attach("Assets/Cappuccino/Shaders/2DShader.frag", ShaderStage::Fragment);
		r2DStorage.quadShader->compile();
		r2DStorage.quadShader->bind();
		r2DStorage.quadShader->setUniform<Int>("uTextureSlot", 0);

		r2DStorage.textShader = Shader::create("Text Default");
		r2DStorage.textShader->attach("Assets/Cappuccino/Shaders/TextShader.vert", ShaderStage::Vertex);
		r2DStorage.textShader->attach("Assets/Cappuccino/Shaders/TextShader.frag", ShaderStage::Fragment);
		r2DStorage.textShader->compile();
		r2DStorage.textShader->bind();
		r2DStorage.textShader->setUniform<Int>("uTextureSlot", 0);

		r2DStorage.mainBufferShader = Shader::create("2D Framebuffer Default");
		r2DStorage.mainBufferShader->attach("Assets/Cappuccino/Shaders/FramebufferShader.vert", ShaderStage::Vertex);
		r2DStorage.mainBufferShader->attach("Assets/Cappuccino/Shaders/FramebufferShader.frag", ShaderStage::Fragment);
		r2DStorage.mainBufferShader->compile();
		r2DStorage.mainBufferShader->bind();
		r2DStorage.mainBufferShader->setUniform<Int>("uTextureSlot", 0);
	}

	// --------------------------------------------------
	// ----- Textures init ------------------------------
	// --------------------------------------------------
	
	{
		r2DStorage.whiteTexture = Texture2D::create(1, 1, &TextureDefaults::whiteTexture);
	}

	// --------------------------------------------------
	// ----- Main framebuffer ---------------------------
	// --------------------------------------------------
	
	{
		r2DStorage.mainBuffer = Framebuffer::create(window->getWidth(), window->getHeight());
		r2DStorage.mainBuffer->setName("Main 2D Framebuffer");
		const Attachment mainColour = { AttachmentType::Texture, InternalFormat::RGBA8 };
		const Attachment depthStencil = { AttachmentType::RenderBuffer, InternalFormat::Depth24Stencil8 };
		r2DStorage.mainBuffer->addAttachment(AttachmentTarget::Colour0, mainColour);
		r2DStorage.mainBuffer->addAttachment(AttachmentTarget::DepthStencil, depthStencil);
	}
	
}

void Renderer2D::shutdown() {}

void Renderer2D::onWindowResized(const uint32_t width, const uint32_t height) {
	r2DStorage.defaultCamera.setProjection(0, static_cast<float>(width), 0, static_cast<float>(height));
	r2DStorage.mainBuffer->resize(width, height);
}

void Renderer2D::start() {
	start(r2DStorage.defaultCamera);
}

void Renderer2D::start(const OrthographicCamera& camera) {
	RenderCommand::setCullingMode(CullMode::None);
	
	r2DStorage.quadShader->bind();
	r2DStorage.quadShader->setUniform<Mat4>("uViewProjection", camera.getViewProjection());

	r2DStorage.textShader->bind();
	r2DStorage.textShader->setUniform<Mat4>("uProjection", camera.getProjectionMatrix());
}

void Renderer2D::drawQuad(const Quad& quad) {
	CAPP_ASSERT(quad.texture != nullptr, "No texture in textured quad!");
	r2DStorage.quadQueue.push_back(quad);
}

void Renderer2D::drawText(const Ref<Text>& text) {
	r2DStorage.textQueue.push_back(text);
}

void Renderer2D::finish() {

	r2DStorage.mainBuffer->bind();
	RenderCommand::setViewport(0, 0, r2DStorage.mainBuffer->getWidth(), r2DStorage.mainBuffer->getHeight());
	// CLEAR WITH ALPHA AT 0.0f SO OTHER LAYERS' FRAMEBUFFERS CAN BE SEEN
	RenderCommand::setClearColour(0.0f, 0.0f, 0.0f, 0.0f);
	RenderCommand::clearScreen();
	RenderCommand::enableDepthTesting();
	RenderCommand::enableBlending();
	RenderCommand::setSeparateBlendFunction(SourceFactor::SourceAlpha, DestinationFactor::OneMinusSourceAlpha, SourceFactor::One, DestinationFactor::OneMinusSourceAlpha);
	
	r2DStorage.quadShader->bind();
	Transform transform;

	std::sort(r2DStorage.quadQueue.begin(), r2DStorage.quadQueue.end(), [](const Quad& first, const Quad& second)->bool {
		return first.zIndex < second.zIndex;
	});
	
	while(!r2DStorage.quadQueue.empty()) {
		auto quad = r2DStorage.quadQueue.front();
		CAPP_ASSERT(quad.texture != nullptr, "No texture in textured quad!");
		quad.texture->bind(0);

		transform.setPosition(quad.zIndexedPosition).setScale(quad.dimensions.x, quad.dimensions.y, 0.0f);

		r2DStorage.quadShader->setUniform<Mat4>("uTransform", transform.getWorldTransform());
		r2DStorage.quadShader->setUniform<Vec4>("uColour", quad.tint);
		r2DStorage.quadShader->setUniform<Float>("uTileFactor", quad.tilingFactor);

		r2DStorage.quadMesh->getVAO()->bind();
		RenderCommand::drawIndexed(r2DStorage.quadMesh->getVAO());

		r2DStorage.quadQueue.pop_front();
	}

	r2DStorage.textShader->bind();
	while(!r2DStorage.textQueue.empty()) {
		const auto& text = r2DStorage.textQueue.front();
		r2DStorage.textShader->setUniform<Vec4>("uTextColour", text->getTextColour());

		auto tempPos = text->getTransform().getPosition();
		for(auto ch = text->getText().begin(); ch != text->getText().end(); ++ch) {
			const Glyph glyph = text->getFont()->getCharacter(*ch);
			if(glyph.texture) {
				const float x = tempPos.x + glyph.bearing.x * text->getTransform().getScale().x;
				const float y = tempPos.y + static_cast<float>(glyph.size.y - glyph.bearing.y) * text->getTransform().getScale().y;
				const float w = glyph.size.x * text->getTransform().getScale().x;
				const float h = glyph.size.y * text->getTransform().getScale().y;

				std::array<float, 16> vertices = { {
					x,     y,       0.0f, 1.0f,
					x + w, y,       1.0f, 1.0f,
					x + w, y - h,   1.0f, 0.0f,
					x,     y - h,   0.0f, 0.0f
				} };

				std::array<uint32_t, 6> indices = {
					0, 1, 2,
					0, 2, 3
				};

				
				glyph.texture->bind(0);
				text->getVAO()->bind();
				text->getVAO()->getVertexBuffers().at(0)->setBufferData(vertices);
				text->getVAO()->getIndexBuffer()->setBufferData(indices);

				RenderCommand::drawIndexed(text->getVAO());
			}
			// Advance cursors for next glyph (note that advance is number of 1/64 pixels)
			tempPos.x += static_cast<float>(glyph.advance >> 6)* text->getTransform().getScale().x;  // Bit shift by 6 to get value in pixels (2^6 = 64)
		}

		r2DStorage.textQueue.pop_front();
	}

	r2DStorage.mainBuffer->unbind();
	RenderCommand::disableDepthTesting();
	RenderCommand::setBlendFunction(SourceFactor::SourceAlpha, DestinationFactor::OneMinusSourceAlpha);
	
	RenderCommand::setViewport(0, 0, r2DStorage.mainBuffer->getWidth(), r2DStorage.mainBuffer->getHeight());
	r2DStorage.mainBufferShader->bind();
	r2DStorage.mainBuffer->getTextureAttachment(AttachmentTarget::Colour0)->bind(0);
	r2DStorage.fullscreenQuad->getVAO()->bind();
	RenderCommand::drawIndexed(r2DStorage.fullscreenQuad->getVAO());

	
	for(uint32_t i = 0; i < 24; ++i) {
		Texture2D::unbind(i);
	}
	Shader::unbind();
	VertexArray::unbind();
}