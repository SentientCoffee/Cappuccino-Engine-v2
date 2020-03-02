#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Renderer.h"

#include "Cappuccino/Core/Application.h"
#include "Cappuccino/Rendering/RenderCommand.h"

using namespace Capp;

struct RendererStorage {
	PerspectiveCamera defaultCamera;
	PerspectiveCamera perspectiveCamera;
	
	Shader* defaultShader = nullptr;
	Shader* hitboxShader = nullptr;

	Lights defaultLights;
	Lights activeLights;

	Mesh* skyboxMesh = nullptr;
	Shader* skyboxShader = nullptr;
	TextureCubemap* defaultSkybox = nullptr;
	TextureCubemap* activeSkybox = nullptr;

	Framebuffer* mainBuffer = nullptr;
	Shader* mainBufferShader = nullptr;
	Mesh* fullscreenQuad = nullptr;

	//std::vector<std::pair<Mesh*, Material*>> renderList;
	std::vector<Model*> modelsToRender;
	std::vector<Hitbox*> hitboxesToRender;
};

static RendererStorage* rendererStorage;

void Renderer::init() {
	rendererStorage = new RendererStorage;
	const auto window = Application::getInstance()->getWindow();

	// --------------------------------------------------
	// ----- Camera init --------------------------------
	// --------------------------------------------------

	{
		rendererStorage->defaultCamera.setProjection(60.0f, window->getWidth(), window->getHeight());
		rendererStorage->defaultCamera.setPosition(0.0f, 5.0f, 5.0f);
		rendererStorage->defaultCamera.lookAt(0.0f, 0.0f, 0.0f);
	}

	// --------------------------------------------------
	// ----- Shaders init -------------------------------
	// --------------------------------------------------

	{
		rendererStorage->defaultShader = new Shader("3D Default");
		rendererStorage->defaultShader->attach("Assets/Cappuccino/Shaders/BlinnPhongShader.vert", ShaderStage::Vertex);
		rendererStorage->defaultShader->attach("Assets/Cappuccino/Shaders/BlinnPhongShader.frag", ShaderStage::Fragment);
		rendererStorage->defaultShader->compile();
		
		rendererStorage->hitboxShader = new Shader("Hitbox Default");
		rendererStorage->hitboxShader->attach("Assets/Cappuccino/Shaders/HitboxShader.vert", ShaderStage::Vertex);
		rendererStorage->hitboxShader->attach("Assets/Cappuccino/Shaders/HitboxShader.frag", ShaderStage::Fragment);
		rendererStorage->hitboxShader->compile();
		
		rendererStorage->skyboxShader = new Shader("Skybox Default");
		rendererStorage->skyboxShader->attach("Assets/Cappuccino/Shaders/CubemapShader.vert", ShaderStage::Vertex);
		rendererStorage->skyboxShader->attach("Assets/Cappuccino/Shaders/CubemapShader.frag", ShaderStage::Fragment);
		rendererStorage->skyboxShader->compile();

		rendererStorage->mainBufferShader = new Shader("Framebuffer Default");
		rendererStorage->mainBufferShader->attach("Assets/Cappuccino/Shaders/FramebufferShader.vert", ShaderStage::Vertex);
		rendererStorage->mainBufferShader->attach("Assets/Cappuccino/Shaders/FramebufferShader.frag", ShaderStage::Fragment);
		rendererStorage->mainBufferShader->compile();
	}
	
	// --------------------------------------------------
	// ----- Lights init --------------------------------
	// --------------------------------------------------

	{
		const auto pointLight = new PointLight;
		pointLight->setPosition(0.0f, 2.0f, 5.0f);
		rendererStorage->defaultLights.pointLights = { pointLight };

		const auto dirLight = new DirectionalLight;
		dirLight->setDirection(0.0f, -1.0f, 0.0f);
		rendererStorage->defaultLights.directionalLights = { dirLight };

		const auto spotlight = new Spotlight;
		spotlight->setPosition(rendererStorage->defaultCamera.getPosition()).setDirection(rendererStorage->defaultCamera.getForward());
		rendererStorage->defaultLights.spotlights = { spotlight };

		rendererStorage->activeLights.directionalLights.reserve(50);
		rendererStorage->activeLights.pointLights.reserve(50);
		rendererStorage->activeLights.spotlights.reserve(50);
	}
	
	// --------------------------------------------------
	// ----- Skybox cubemap init ------------------------
	// --------------------------------------------------

	{
		const std::vector<Vertex> vertices = {
			{ { -1.0f, -1.0f, -1.0f } },
			{ {  1.0f, -1.0f, -1.0f } },
			{ { -1.0f, -1.0f,  1.0f } },
			{ {  1.0f, -1.0f,  1.0f } },
			{ { -1.0f,  1.0f, -1.0f } },
			{ {  1.0f,  1.0f, -1.0f } },
			{ { -1.0f,  1.0f,  1.0f } },
			{ {  1.0f,  1.0f,  1.0f } }
		};

		const std::vector<unsigned> indices = {
			0, 1, 2,	2, 1, 3,	// bottom
			4, 6, 5,	6, 7, 5,	// top
			2, 3, 6,	6, 3, 7,	// front
			0, 1, 4,	4, 1, 5,	// back
			2, 4, 0,	2, 6, 4,	// left
			3, 5, 1,	3, 7, 5		// right
		
		};

		rendererStorage->skyboxMesh = new Mesh("Skybox", vertices, indices);

		const std::vector<std::string> filepaths = {
			"Assets/Cappuccino/Textures/Skybox/corona_lf.png",
			"Assets/Cappuccino/Textures/Skybox/corona_rt.png",
			"Assets/Cappuccino/Textures/Skybox/corona_up.png",
			"Assets/Cappuccino/Textures/Skybox/corona_dn.png",
			"Assets/Cappuccino/Textures/Skybox/corona_ft.png",
			"Assets/Cappuccino/Textures/Skybox/corona_bk.png"
		};

		rendererStorage->defaultSkybox = new TextureCubemap(filepaths);
	}
	// --------------------------------------------------
	// ----- Main framebuffer ---------------------------
	// --------------------------------------------------

	{
		rendererStorage->mainBuffer = new Framebuffer(window->getWidth(), window->getHeight());
		rendererStorage->mainBuffer->setName("Main Framebuffer");
		const Attachment mainColour = { AttachmentType::Texture, AttachmentFormat::RGBA8 };
		const Attachment depthStencil = { AttachmentType::RenderBuffer, AttachmentFormat::Depth24Stencil8 };
		rendererStorage->mainBuffer->addAttachment(AttachmentTarget::Colour0, mainColour);
		rendererStorage->mainBuffer->addAttachment(AttachmentTarget::DepthStencil, depthStencil);
	}
	
	// --------------------------------------------------
	// ----- Fullscreen Quad ----------------------------
	// --------------------------------------------------

	{
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

		rendererStorage->fullscreenQuad = new Mesh("Main framebuffer fullscreen quad", vertices, indices);
	}
	
}

void Renderer::shutdown() {
	delete rendererStorage;
}

void Renderer::onWindowResized(const unsigned width, const unsigned height) {
	rendererStorage->defaultCamera.setProjection(60.0f, width, height);
	rendererStorage->mainBuffer->resize(width, height);
}

void Renderer::start() {
	start(rendererStorage->defaultCamera, rendererStorage->defaultLights, rendererStorage->defaultSkybox);
}

void Renderer::start(const PerspectiveCamera& camera, const Lights& lights, const std::optional<TextureCubemap*>& skybox) {
	rendererStorage->perspectiveCamera = camera;
	rendererStorage->activeLights.directionalLights = lights.directionalLights;
	rendererStorage->activeLights.pointLights = lights.pointLights;
	rendererStorage->activeLights.spotlights = lights.spotlights;
	rendererStorage->activeSkybox = skybox.value_or(nullptr);

	//rendererStorage->renderList.clear();
	//rendererStorage->renderList.reserve(400);

	rendererStorage->modelsToRender.clear();
	rendererStorage->modelsToRender.reserve(400);
	
	if(Hitbox::shouldDraw()) {
		rendererStorage->hitboxesToRender.clear();
		rendererStorage->hitboxesToRender.reserve(400);
	}
}

void Renderer::addToRenderList(VertexArray* vertexArray, Shader* shader) {
	shader->bind();
	vertexArray->bind();
	if(vertexArray->getIndexBuffer() != nullptr) {
		RenderCommand::drawIndexed(vertexArray);
	}
	else {
		RenderCommand::drawArray(vertexArray);
	}
}

void Renderer::addToRenderList(VertexArray* vertexArray, Material* material) {
	material->apply();

	material->getShader()->bind();
	material->getShader()->setUniform<Vec3>("uCameraPosition", rendererStorage->perspectiveCamera.getPosition());
	material->getShader()->setUniform<Mat4>("uViewProjection", rendererStorage->perspectiveCamera.getViewProjection());
	material->getShader()->setUniform<Int>("uNumPointLights", static_cast<int>(rendererStorage->activeLights.pointLights.size()));
	material->getShader()->setUniform<Int>("uNumDirectionalLights", static_cast<int>(rendererStorage->activeLights.directionalLights.size()));
	material->getShader()->setUniform<Int>("uNumSpotlights", static_cast<int>(rendererStorage->activeLights.spotlights.size()));

	material->getShader()->setUniform<Vec3>("uAmbientColour", glm::vec3(0.2f, 0.4f, 0.8f));
	material->getShader()->setUniform<Float>("uAmbientPower", 0.3f);

	for(unsigned i = 0; i < rendererStorage->activeLights.directionalLights.size(); ++i) {
		material->getShader()->setUniform<Vec3>("uDirectionalLights[" + std::to_string(i) + "].direction", rendererStorage->activeLights.directionalLights[i]->getDirection());
		material->getShader()->setUniform<Vec3>("uDirectionalLights[" + std::to_string(i) + "].colour", rendererStorage->activeLights.directionalLights[i]->getColour());
	}

	for(unsigned i = 0; i < rendererStorage->activeLights.pointLights.size(); ++i) {
		material->getShader()->setUniform<Vec3>("uPointLights[" + std::to_string(i) + "].position", rendererStorage->activeLights.pointLights[i]->getPosition());
		material->getShader()->setUniform<Vec3>("uPointLights[" + std::to_string(i) + "].colour", rendererStorage->activeLights.pointLights[i]->getColour());
		material->getShader()->setUniform<Float>("uPointLights[" + std::to_string(i) + "].attenuation", rendererStorage->activeLights.pointLights[i]->getAttenuation());
	}

	for(unsigned i = 0; i < rendererStorage->activeLights.spotlights.size(); ++i) {
		material->getShader()->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].position", rendererStorage->activeLights.spotlights[i]->getPosition());
		material->getShader()->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].direction", rendererStorage->activeLights.spotlights[i]->getDirection());
		material->getShader()->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].colour", rendererStorage->activeLights.spotlights[i]->getColour());
		material->getShader()->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].attenuation", rendererStorage->activeLights.spotlights[i]->getAttenuation());
		material->getShader()->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].innerCutoffAngle", glm::cos(glm::radians(rendererStorage->activeLights.spotlights[i]->getInnerCutoffAngle())));
		material->getShader()->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].outerCutoffAngle", glm::cos(glm::radians(rendererStorage->activeLights.spotlights[i]->getOuterCutoffAngle())));
	}
	
	addToRenderList(vertexArray, material->getShader());
}

void Renderer::addToRenderList(Mesh* mesh, Shader* shader) {
	addToRenderList(mesh->getVAO(), shader);
}

void Renderer::addToRenderList(Mesh* mesh, Material* material) {
	//rendererStorage->renderList.emplace_back(mesh, material);
	addToRenderList(mesh->getVAO(), material->getShader());
}

void Renderer::addToRenderList(Model* model) {
	//model->getMaterial()->getShader()->bind();
	//model->getMaterial()->getShader()->setUniformMat4("uTransform", model->getTransform().getWorldTransform());
	//model->getMaterial()->getShader()->setUniformMat3("uNormalMatrix", glm::mat3(glm::transpose(glm::inverse(model->getTransform().getWorldTransform()))));
	rendererStorage->modelsToRender.push_back(model);

	//addToRenderList(model->getMesh(), model->getMaterial());
}

void Renderer::addToRenderList(Hitbox* hitbox) {
	//if(!Hitbox::shouldDraw()) {
	//	return;
	//}

	//RenderCommand::disableCulling();
	//RenderCommand::setDrawMode(DrawMode::Line);

	//rendererStorage->hitboxShader->bind();
	//rendererStorage->hitboxShader->setUniformVec4("uHitboxColour", { 1.0f, 0.0f, 0.0f, 1.0f });
	//rendererStorage->hitboxShader->setUniformMat4("uTransform", hitbox->getTransform().getWorldTransform());
	//rendererStorage->hitboxShader->setUniformMat4("uViewProjection", rendererStorage->perspectiveCamera.getViewProjection());

	//hitbox->getVAO()->bind();
	//RenderCommand::drawIndexed(hitbox->getVAO());

	//RenderCommand::setDrawMode(DrawMode::Fill);
	//RenderCommand::enableCulling();

	rendererStorage->hitboxesToRender.push_back(hitbox);
}

void Renderer::addToRenderList(const RigidBody& rigidBody) {
	if(!Hitbox::shouldDraw()) {
		return;
	}
	
	for(auto hitbox : rigidBody) {
		addToRenderList(hitbox);
	}
}

void Renderer::addToRenderList(GameObject* gameObject) {
	for(auto model : gameObject->getModels()) {
		addToRenderList(model);
	}

	if(gameObject->isVisible()) {
		addToRenderList(gameObject->getRigidBody());
	}
}

void Renderer::finish(const PostPasses& postProcessing) {

	rendererStorage->mainBuffer->bind();
	RenderCommand::setViewport(0, 0, rendererStorage->mainBuffer->getWidth(), rendererStorage->mainBuffer->getHeight());
	// CLEAR WITH ALPHA AT 0.0f SO OTHER LAYERS' FRAMEBUFFERS CAN BE SEEN
	RenderCommand::setClearColour(0.0f, 0.0f, 0.0f, 0.0f);
	RenderCommand::clearScreen();
	RenderCommand::enableDepthTesting();
	RenderCommand::enableCulling();
	RenderCommand::setSeparateBlendFunction(SourceFactor::SourceAlpha, DestinationFactor::OneMinusSourceAlpha, SourceFactor::One, DestinationFactor::OneMinusSourceAlpha);

	// Sort by shader ID (to avoid rebinding the same shader)
	std::sort(rendererStorage->modelsToRender.begin(), rendererStorage->modelsToRender.end(), [](Model* first, const Model* second)->bool {
		if(second->getMaterial() == nullptr || second->getMesh() == nullptr) return false;
		if(first->getMaterial() == nullptr || first->getMesh() == nullptr) return true;
		//if(first.second->RasterState.Blending.BlendEnabled & !second.second->RasterState.Blending.BlendEnabled) return false;
		//if(!first.second->RasterState.Blending.BlendEnabled & second.second->RasterState.Blending.BlendEnabled) return true;
		if(first->getMaterial()->getShader() != second->getMaterial()->getShader()) return first->getMaterial()->getShader() < second->getMaterial()->getShader();
		return first->getMaterial() < second->getMaterial();
	});
	
	// Render scene
	{
		Shader* boundShader = nullptr;

		for(auto model : rendererStorage->modelsToRender) {
			Shader* shader = model->getMaterial()->getShader();
		
			if(shader == nullptr) {
				CAPP_PRINT_WARNING("No shader bound, rendering with default shader");
				shader = rendererStorage->defaultShader;
			}

			if(boundShader != shader) {
				boundShader = shader;
				boundShader->bind();
				
				shader->setUniform<Vec3>("uCameraPosition", rendererStorage->perspectiveCamera.getPosition());
				
				shader->setUniform<Int>("uNumPointLights", static_cast<int>(rendererStorage->activeLights.pointLights.size()));
				shader->setUniform<Int>("uNumDirectionalLights", static_cast<int>(rendererStorage->activeLights.directionalLights.size()));
				shader->setUniform<Int>("uNumSpotlights", static_cast<int>(rendererStorage->activeLights.spotlights.size()));

				shader->setUniform<Vec3>("uAmbientColour", glm::vec3(0.2f, 0.4f, 0.6f));
				shader->setUniform<Float>("uAmbientPower", 0.3f);

				for(unsigned i = 0; i < rendererStorage->activeLights.directionalLights.size(); ++i) {
					shader->setUniform<Vec3>("uDirectionalLights[" + std::to_string(i) + "].direction", rendererStorage->activeLights.directionalLights[i]->getDirection());
					shader->setUniform<Vec3>("uDirectionalLights[" + std::to_string(i) + "].colour", rendererStorage->activeLights.directionalLights[i]->getColour());
				}

				for(unsigned i = 0; i < rendererStorage->activeLights.pointLights.size(); ++i) {
					shader->setUniform<Vec3>("uPointLights[" + std::to_string(i) + "].position", rendererStorage->activeLights.pointLights[i]->getPosition());
					shader->setUniform<Vec3>("uPointLights[" + std::to_string(i) + "].colour", rendererStorage->activeLights.pointLights[i]->getColour());
					shader->setUniform<Float>("uPointLights[" + std::to_string(i) + "].attenuation", rendererStorage->activeLights.pointLights[i]->getAttenuation());
				}

				for(unsigned i = 0; i < rendererStorage->activeLights.spotlights.size(); ++i) {
					shader->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].position", rendererStorage->activeLights.spotlights[i]->getPosition());
					shader->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].direction", rendererStorage->activeLights.spotlights[i]->getDirection());
					shader->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].colour", rendererStorage->activeLights.spotlights[i]->getColour());
					shader->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].attenuation", rendererStorage->activeLights.spotlights[i]->getAttenuation());
					shader->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].innerCutoffAngle", glm::cos(glm::radians(rendererStorage->activeLights.spotlights[i]->getInnerCutoffAngle())));
					shader->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].outerCutoffAngle", glm::cos(glm::radians(rendererStorage->activeLights.spotlights[i]->getOuterCutoffAngle())));
				}
			}

			shader->setUniform<Mat4>("uViewProjection", rendererStorage->perspectiveCamera.getViewProjection());
			shader->setUniform<Mat4>("uTransform", model->getTransform().getWorldTransform());
			shader->setUniform<Mat3>("uNormalMatrix", glm::mat3(glm::transpose(glm::inverse(model->getTransform().getWorldTransform()))));
			model->getMaterial()->apply();
			
			model->getMesh()->getVAO()->bind();
			if(model->getMesh()->getVAO()->getIndexBuffer() != nullptr) {
				RenderCommand::drawIndexed(model->getMesh()->getVAO());
			}
			else {
				RenderCommand::drawArray(model->getMesh()->getVAO());
			}
		}		

		
		if(Hitbox::shouldDraw()) {
			RenderCommand::disableCulling();
			RenderCommand::setDrawMode(DrawMode::Line);
			
			for(auto hitbox : rendererStorage->hitboxesToRender) {
				rendererStorage->hitboxShader->bind();
				rendererStorage->hitboxShader->setUniform<Vec4>("uHitboxColour", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				rendererStorage->hitboxShader->setUniform<Mat4>("uTransform", hitbox->getTransform().getWorldTransform());
				rendererStorage->hitboxShader->setUniform<Mat4>("uViewProjection", rendererStorage->perspectiveCamera.getViewProjection());

				hitbox->getVAO()->bind();
				RenderCommand::drawIndexed(hitbox->getVAO());
			}

			RenderCommand::setDrawMode(DrawMode::Fill);
			RenderCommand::enableCulling();
		}
		
		if(rendererStorage->activeSkybox != nullptr) {
			RenderCommand::disableCulling();
			RenderCommand::setDepthTestFunction(DepthTestFunction::LessThanOrEqual);
			RenderCommand::disableDepthMask();

			rendererStorage->skyboxShader->bind();
			const glm::mat4 viewProjection = rendererStorage->perspectiveCamera.getProjectionMatrix() * glm::mat4(glm::mat3(rendererStorage->perspectiveCamera.getViewMatrix()));
			rendererStorage->skyboxShader->setUniform<Mat4>("uViewProjection", viewProjection);

			rendererStorage->activeSkybox->bind(0);
			rendererStorage->skyboxShader->setUniform<Int>("uSkybox", 0);

			rendererStorage->skyboxMesh->getVAO()->bind();
			RenderCommand::drawIndexed(rendererStorage->skyboxMesh->getVAO());

			RenderCommand::enableDepthMask();
			RenderCommand::setDepthTestFunction(DepthTestFunction::LessThan);
		}
	}

	rendererStorage->mainBuffer->unbind();
	RenderCommand::disableDepthTesting();

	const auto window = Application::getInstance()->getWindow();
	
	Framebuffer* lastPass = rendererStorage->mainBuffer;
	for(auto pass : postProcessing) {
		pass.buffer->validateFramebuffer();
		pass.buffer->bind();
		
		//RenderCommand::clearScreen(ClearFlags::Colour);
		RenderCommand::setViewport(0, 0, pass.buffer->getWidth(), pass.buffer->getHeight());
		
		pass.shader->bind();
		lastPass->getAttachment(AttachmentTarget::Colour0)->bind(0);
		pass.shader->setUniform<Int>("uImage", 0);
		pass.shader->setUniform<Vec2>("uScreenSize", glm::vec2(window->getWidth(), window->getHeight()));
		
		rendererStorage->fullscreenQuad->getVAO()->bind();
		RenderCommand::drawIndexed(rendererStorage->fullscreenQuad->getVAO());

		pass.buffer->unbind();
		lastPass = pass.buffer;
	}

	RenderCommand::setBlendFunction(SourceFactor::SourceAlpha, DestinationFactor::OneMinusSourceAlpha);
	
	//lastPass->bind(FramebufferBinding::ReadOnly);
	//Framebuffer::blit({ 0, 0, rendererStorage->mainBuffer->getWidth(), rendererStorage->mainBuffer->getHeight() },
	//	{ 0, 0, window->getWidth(), window->getHeight() },
	//	ClearFlags::All, MagFilter::Nearest);
	//lastPass->unbind();
	
	RenderCommand::setViewport(0, 0, lastPass->getWidth(), lastPass->getHeight());
	rendererStorage->mainBufferShader->bind();
	lastPass->getAttachment(AttachmentTarget::Colour0)->bind(0);
	rendererStorage->fullscreenQuad->getVAO()->bind();
	RenderCommand::drawIndexed(rendererStorage->fullscreenQuad->getVAO());
	
	for(unsigned i = 0; i < 24; ++i) {
		Texture2D::unbind(i);
	}
	Shader::unbind();
	TextureCubemap::unbind();
	VertexArray::unbind();
}