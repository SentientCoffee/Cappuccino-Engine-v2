#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Renderer.h"

#include "Cappuccino/Core/Application.h"

#include "Cappuccino/Rendering/RenderCommand.h"
#include "Cappuccino/Rendering/Textures/Texture1D.h"
#include "Cappuccino/Rendering/Textures/Texture3D.h"

using namespace Capp;

#define MAX_DIR_LIGHTS 10
#define MAX_POINT_LIGHTS 50
#define MAX_SPOTLIGHTS 50

struct RendererStorage {
	PerspectiveCamera defaultCamera;
	PerspectiveCamera perspectiveCamera;

	Shader* blinnPhongShader     = nullptr;
	Shader* hitboxShader         = nullptr;
	Shader* skyboxShader         = nullptr;
	Shader* framebufferShader    = nullptr;
	Shader* shadowPass           = nullptr;
	Shader* gBufferPass          = nullptr;
	Shader* deferredLightingPass = nullptr;

	Lights defaultLights;
	Lights activeLights;

	Mesh* skyboxMesh     = nullptr;
	Mesh* fullscreenQuad = nullptr;

	TextureCubemap* defaultSkybox = nullptr;
	TextureCubemap* activeSkybox  = nullptr;

	Framebuffer* gBuffer            = nullptr;
	Framebuffer* deferredComposite  = nullptr;
	Framebuffer* accumulationBuffer = nullptr;

	PostProcessingPass gammaCorrection = { nullptr, nullptr };

	std::deque<Model*> modelRenderQueue;
	std::deque<Hitbox*> hitboxRenderQueue;

	float gamma = 2.2f;
};

static RendererStorage* rStorage;

void Renderer::init() {
	rStorage = new RendererStorage;
	const auto window = Application::getInstance()->getWindow();

	// --------------------------------------------------
	// ----- Camera init --------------------------------
	// --------------------------------------------------

	{
		rStorage->defaultCamera.setProjection(60.0f, window->getWidth(), window->getHeight());
		rStorage->defaultCamera.setPosition(0.0f, 5.0f, 5.0f);
		rStorage->defaultCamera.lookAt(0.0f, 0.0f, 0.0f);
	}

	// --------------------------------------------------
	// ----- Shaders init -------------------------------
	// --------------------------------------------------

	{
		rStorage->blinnPhongShader = new Shader("Blinn Phong Default");
		rStorage->blinnPhongShader->attach("Assets/Cappuccino/Shaders/SimpleGeometry.vert", ShaderStage::Vertex);
		rStorage->blinnPhongShader->attach("Assets/Cappuccino/Shaders/BlinnPhongShader.frag", ShaderStage::Fragment);
		rStorage->blinnPhongShader->compile();

		rStorage->hitboxShader = new Shader("Hitbox Default");
		rStorage->hitboxShader->attach("Assets/Cappuccino/Shaders/HitboxShader.vert", ShaderStage::Vertex);
		rStorage->hitboxShader->attach("Assets/Cappuccino/Shaders/HitboxShader.frag", ShaderStage::Fragment);
		rStorage->hitboxShader->compile();

		rStorage->skyboxShader = new Shader("Skybox Default");
		rStorage->skyboxShader->attach("Assets/Cappuccino/Shaders/CubemapShader.vert", ShaderStage::Vertex);
		rStorage->skyboxShader->attach("Assets/Cappuccino/Shaders/CubemapShader.frag", ShaderStage::Fragment);
		rStorage->skyboxShader->compile();

		rStorage->framebufferShader = new Shader("Framebuffer Default");
		rStorage->framebufferShader->attach("Assets/Cappuccino/Shaders/FramebufferShader.vert", ShaderStage::Vertex);
		rStorage->framebufferShader->attach("Assets/Cappuccino/Shaders/FramebufferShader.frag", ShaderStage::Fragment);
		rStorage->framebufferShader->compile();

		rStorage->shadowPass = new Shader("Shadow Mapping Default");
		rStorage->shadowPass->attach("Assets/Cappuccino/Shaders/SimpleShadow.vert", ShaderStage::Vertex);
		rStorage->shadowPass->compile();
		
		rStorage->gBufferPass = new Shader("Deferred GBuffer Default");
		rStorage->gBufferPass->attach("Assets/Cappuccino/Shaders/SimpleGeometry.vert", ShaderStage::Vertex);
		rStorage->gBufferPass->attach("Assets/Cappuccino/Shaders/GBufferPass.frag", ShaderStage::Fragment);
		rStorage->gBufferPass->compile();

		rStorage->deferredLightingPass = new Shader("Deferred Lighting Default");
		rStorage->deferredLightingPass->attach("Assets/Cappuccino/Shaders/DeferredLightingShader.vert", ShaderStage::Vertex);
		rStorage->deferredLightingPass->attach("Assets/Cappuccino/Shaders/DeferredLightingShader.frag", ShaderStage::Fragment);
		rStorage->deferredLightingPass->compile();
	}

	// --------------------------------------------------
	// ----- Lights init --------------------------------
	// --------------------------------------------------

	{
		const auto pointLight = new PointLight;
		pointLight->setPosition(0.0f, 2.0f, 5.0f);
		rStorage->defaultLights.pointLights = { pointLight };

		const auto dirLight = new DirectionalLight;
		dirLight->setDirection(0.0f, -1.0f, 0.0f);
		rStorage->defaultLights.directionalLights = { dirLight };

		const auto spotlight = new Spotlight;
		spotlight->setPosition(rStorage->defaultCamera.getPosition()).setDirection(rStorage->defaultCamera.getForward());
		rStorage->defaultLights.spotlights = { spotlight };

		rStorage->activeLights.directionalLights.reserve(MAX_DIR_LIGHTS);
		rStorage->activeLights.pointLights.reserve(MAX_POINT_LIGHTS);
		rStorage->activeLights.spotlights.reserve(MAX_SPOTLIGHTS);
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

		rStorage->skyboxMesh = new Mesh("Skybox", vertices, indices);

		const std::vector<std::string> filepaths = {
			"Assets/Cappuccino/Textures/Skybox/corona_lf.png",
			"Assets/Cappuccino/Textures/Skybox/corona_rt.png",
			"Assets/Cappuccino/Textures/Skybox/corona_up.png",
			"Assets/Cappuccino/Textures/Skybox/corona_dn.png",
			"Assets/Cappuccino/Textures/Skybox/corona_ft.png",
			"Assets/Cappuccino/Textures/Skybox/corona_bk.png"
		};

		rStorage->defaultSkybox = new TextureCubemap(filepaths);
	}

	// --------------------------------------------------
	// ----- Fullscreen quad ----------------------------
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

		rStorage->fullscreenQuad = new Mesh("Main framebuffer fullscreen quad", vertices, indices);
	}
	
	// --------------------------------------------------
	// ----- Geometry buffer (GBuffer) ------------------
	// --------------------------------------------------

	{
		rStorage->gBuffer = new Framebuffer(window->getWidth(), window->getHeight());
		rStorage->gBuffer->setName("GBuffer");
		const Attachment position     = { AttachmentType::Texture, InternalFormat::RGBA32F, { WrapMode::Repeat, MinFilter::Nearest, MagFilter::Linear } };
		const Attachment normal       = { AttachmentType::Texture, InternalFormat::RGBA16F, { WrapMode::Repeat, MinFilter::Nearest, MagFilter::Linear } };
		const Attachment albedo       = { AttachmentType::Texture, InternalFormat::RGBA8,   { WrapMode::Repeat, MinFilter::Nearest, MagFilter::Linear } };
		const Attachment specRough    = { AttachmentType::Texture, InternalFormat::RGBA8,   { WrapMode::Repeat, MinFilter::Nearest, MagFilter::Linear } };
		const Attachment emission     = { AttachmentType::Texture, InternalFormat::RGBA8,   { WrapMode::Repeat, MinFilter::Nearest, MagFilter::Linear } };
		const Attachment depthStencil = { AttachmentType::RenderBuffer, InternalFormat::Depth24_Stencil8 };

		rStorage->gBuffer->addAttachment(AttachmentTarget::Colour0, position);
		rStorage->gBuffer->addAttachment(AttachmentTarget::Colour1, normal);
		rStorage->gBuffer->addAttachment(AttachmentTarget::Colour2, albedo);
		rStorage->gBuffer->addAttachment(AttachmentTarget::Colour3, specRough);
		rStorage->gBuffer->addAttachment(AttachmentTarget::Colour4, emission);
		rStorage->gBuffer->addAttachment(AttachmentTarget::DepthStencil, depthStencil);
	}

	// --------------------------------------------------
	// ----- Deferred composite framebuffer -------------
	// --------------------------------------------------

	{
		rStorage->deferredComposite = new Framebuffer(window->getWidth(), window->getHeight());
		rStorage->deferredComposite->setName("Deferred Composite Framebuffer");
		const Attachment mainColour   = { AttachmentType::Texture, InternalFormat::RGBA8 };
		const Attachment depthStencil = { AttachmentType::RenderBuffer, InternalFormat::Depth24_Stencil8 };
		rStorage->deferredComposite->addAttachment(AttachmentTarget::Colour0, mainColour);
		rStorage->deferredComposite->addAttachment(AttachmentTarget::DepthStencil, depthStencil);
	}

	// --------------------------------------------------
	// ----- Gamma correction post process pass ---------
	// --------------------------------------------------
	
	{
		rStorage->gammaCorrection.buffer = new Framebuffer(window->getWidth(), window->getHeight());
		const Attachment mainColour = { AttachmentType::Texture, InternalFormat::RGB8 };
		rStorage->gammaCorrection.buffer->addAttachment(AttachmentTarget::Colour0, mainColour);
		
		rStorage->gammaCorrection.shader = new Shader("Gamma Correction");
		rStorage->gammaCorrection.shader->attach("Assets/Cappuccino/Shaders/PostProcessing/PostProcessingShader.vert", ShaderStage::Vertex);
		rStorage->gammaCorrection.shader->attach("Assets/Cappuccino/Shaders/PostProcessing/GammaCorrectionShader.frag", ShaderStage::Fragment);
		rStorage->gammaCorrection.shader->compile();
	}
}

void Renderer::shutdown() {
	delete rStorage;
}

void Renderer::onWindowResized(const unsigned width, const unsigned height) {
	rStorage->defaultCamera.setProjection(60.0f, width, height);
	rStorage->deferredComposite->resize(width, height);
	rStorage->gBuffer->resize(width, height);
}

void Renderer::setGamma(const float gamma) {
	rStorage->gamma = gamma;
}

void Renderer::start() {
	start(rStorage->defaultCamera, rStorage->defaultLights, rStorage->defaultSkybox);
}

void Renderer::start(const PerspectiveCamera& camera, const Lights& lights, const Skybox& skybox) {
	rStorage->perspectiveCamera              = camera;
	rStorage->activeLights.directionalLights = lights.directionalLights;
	rStorage->activeLights.pointLights       = lights.pointLights;
	rStorage->activeLights.spotlights        = lights.spotlights;
	rStorage->activeSkybox                   = skybox.value_or(nullptr);
}

void Renderer::addToQueue(VertexArray* vertexArray, Shader* shader) {
	shader->bind();
	vertexArray->bind();
	if(vertexArray->getIndexBuffer() != nullptr) {
		RenderCommand::drawIndexed(vertexArray);
	}
	else {
		RenderCommand::drawArray(vertexArray);
	}
}

void Renderer::addToQueue(VertexArray* vertexArray, Material* material) {
	material->apply();

	material->getShader()->bind();
	material->getShader()->setUniform<Vec3>("uCameraPosition", rStorage->perspectiveCamera.getPosition());
	material->getShader()->setUniform<Mat4>("uViewProjection", rStorage->perspectiveCamera.getViewProjection());
	material->getShader()->setUniform<Int>("uNumPointLights", static_cast<int>(rStorage->activeLights.pointLights.size()));
	material->getShader()->setUniform<Int>("uNumDirectionalLights", static_cast<int>(rStorage->activeLights.directionalLights.size()));
	material->getShader()->setUniform<Int>("uNumSpotlights", static_cast<int>(rStorage->activeLights.spotlights.size()));

	material->getShader()->setUniform<Vec3>("uAmbientColour", glm::vec3(0.2f, 0.4f, 0.8f));
	material->getShader()->setUniform<Float>("uAmbientPower", 0.3f);

	for(unsigned i = 0; i < rStorage->activeLights.directionalLights.size(); ++i) {
		material->getShader()->setUniform<Vec3>("uDirectionalLights[" + std::to_string(i) + "].direction", rStorage->activeLights.directionalLights[i]->getDirection());
		material->getShader()->setUniform<Vec3>("uDirectionalLights[" + std::to_string(i) + "].colour", rStorage->activeLights.directionalLights[i]->getColour());
	}

	for(unsigned i = 0; i < rStorage->activeLights.pointLights.size(); ++i) {
		material->getShader()->setUniform<Vec3>("uPointLights[" + std::to_string(i) + "].position", rStorage->activeLights.pointLights[i]->getPosition());
		material->getShader()->setUniform<Vec3>("uPointLights[" + std::to_string(i) + "].colour", rStorage->activeLights.pointLights[i]->getColour());
		material->getShader()->setUniform<Float>("uPointLights[" + std::to_string(i) + "].attenuation", rStorage->activeLights.pointLights[i]->getAttenuation());
	}

	for(unsigned i = 0; i < rStorage->activeLights.spotlights.size(); ++i) {
		material->getShader()->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].position", rStorage->activeLights.spotlights[i]->getPosition());
		material->getShader()->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].direction", rStorage->activeLights.spotlights[i]->getDirection());
		material->getShader()->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].colour", rStorage->activeLights.spotlights[i]->getColour());
		material->getShader()->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].attenuation", rStorage->activeLights.spotlights[i]->getAttenuation());
		material->getShader()->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].innerCutoffAngle", glm::cos(glm::radians(rStorage->activeLights.spotlights[i]->getInnerCutoffAngle())));
		material->getShader()->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].outerCutoffAngle", glm::cos(glm::radians(rStorage->activeLights.spotlights[i]->getOuterCutoffAngle())));
	}

	addToQueue(vertexArray, material->getShader());
}

void Renderer::addToQueue(Mesh* mesh, Shader* shader) {
	addToQueue(mesh->getVAO(), shader);
}

void Renderer::addToQueue(Mesh* mesh, Material* material) {
	rStorage->modelRenderQueue.emplace_back(mesh, material);
}

void Renderer::addToQueue(Model* model) {
	rStorage->modelRenderQueue.push_back(model);
}

void Renderer::addToQueue(GameObject* gameObject) {
	for(auto model : gameObject->getModels()) {
		addToQueue(model);
	}
	if(gameObject->isVisible()) {
		addToQueue(gameObject->getRigidBody());
	}
}

void Renderer::addToQueue(Hitbox* hitbox) {
	rStorage->hitboxRenderQueue.push_back(hitbox);
}

void Renderer::addToQueue(const RigidBody& rigidBody) {
	if(!Hitbox::shouldDraw()) {
		return;
	}
	for(auto hitbox : rigidBody) {
		addToQueue(hitbox);
	}
}

void Renderer::finish(const PostPasses& postProcessing) {

	const auto window = Application::getInstance()->getWindow();

	// --------------------------------------------------
	// ----- Model sorting ------------------------------
	// --------------------------------------------------
	
	std::sort(rStorage->modelRenderQueue.begin(), rStorage->modelRenderQueue.end(), [](Model* first, const Model* second)-> bool {
		if(second->getMaterial() == nullptr || second->getMesh() == nullptr) {
			return false;
		}
		if(first->getMaterial() == nullptr || first->getMesh() == nullptr) {
			return true;
		}
		// TODO: BLENDING
		//if(first.second->RasterState.Blending.BlendEnabled & !second.second->RasterState.Blending.BlendEnabled)
		//	return false;
		//if(!first.second->RasterState.Blending.BlendEnabled & second.second->RasterState.Blending.BlendEnabled)
		//	return true;
		if(first->getMaterial()->getShader() != nullptr && second->getMaterial()->getShader() != nullptr) {
			return first->getMaterial()->getShader()->getRendererID() < second->getMaterial()->getShader()->getRendererID();
		}
		if(first->getMaterial()->getShader() != second->getMaterial()->getShader()) {
			return first->getMaterial()->getShader() < second->getMaterial()->getShader();
		}

		return first->getMaterial() < second->getMaterial();
	});

	// CLEAR WITH ALPHA AT 0.0f SO OTHER LAYERS' FRAMEBUFFERS CAN BE SEEN
	RenderCommand::setClearColour(0.0f, 0.0f, 0.0f, 0.0f);
	RenderCommand::enableDepthTesting();
	RenderCommand::enableCulling();
	RenderCommand::setSeparateBlendFunction(SourceFactor::SourceAlpha, DestinationFactor::OneMinusSourceAlpha, SourceFactor::One, DestinationFactor::OneMinusSourceAlpha);

	// --------------------------------------------------
	// ----- Shadow mapping -----------------------------
	// --------------------------------------------------
	
	{
		RenderCommand::setCullingMode(CullMode::FrontFace);
		std::deque<Light*> allLights;
		
		for(auto light : rStorage->activeLights.directionalLights) {
			const float maxBound = 10.0f;
			light->setProjection(glm::ortho(-maxBound, maxBound, -maxBound, maxBound, -10.0f, 100.0f));
			allLights.push_back(light);
		}

		for(auto light : rStorage->activeLights.pointLights) {
			const auto aspect = static_cast<float>(light->getShadowBuffer()->getWidth()) / static_cast<float>(light->getShadowBuffer()->getHeight());
			light->setProjection(glm::perspective(60.0f, aspect, 0.1f, 100.0f));
			allLights.push_back(light);
		}
		
		for(auto light : rStorage->activeLights.spotlights) {
			const auto aspect = static_cast<float>(light->getShadowBuffer()->getWidth()) / static_cast<float>(light->getShadowBuffer()->getHeight());
			light->setProjection(glm::perspective(light->getOuterCutoffAngle(), aspect, 0.1f, 100.0f));
			allLights.push_back(light);
		}

		while(!allLights.empty()) {
			const auto light = allLights.front();
			light->getShadowBuffer()->bind();
			RenderCommand::setViewport(0, 0, light->getShadowBuffer()->getWidth(), light->getShadowBuffer()->getHeight());
			RenderCommand::clearScreen(ClearFlags::Depth);
			{
				for(auto model : rStorage->modelRenderQueue) {
					if(model->getMesh() == nullptr) {
						CAPP_PRINT_ERROR("Failed to draw model: missing mesh!");
						continue;
					}
					if(model->getMaterial() == nullptr) {
						CAPP_PRINT_ERROR("Failed to draw model: missing material!");
						continue;
					}

					const auto mesh = model->getMesh();

					rStorage->shadowPass->bind();
					rStorage->shadowPass->setUniform<Mat4>("uViewProjection", glm::inverse(light->getTransform().getWorldTransform()) * light->getProjectionMatrix());
					rStorage->shadowPass->setUniform<Mat4>("uTransform", model->getTransform().getWorldTransform());

					mesh->getVAO()->bind();
					if(mesh->getVAO()->getIndexBuffer() != nullptr) {
						RenderCommand::drawIndexed(mesh->getVAO());
					}
					else {
						RenderCommand::drawArray(mesh->getVAO());
					}
				}
			}
			light->getShadowBuffer()->unbind();
			allLights.pop_front();
		}

		RenderCommand::setCullingMode(CullMode::BackFace);
	}

	// --------------------------------------------------
	// ----- Deferred rendering -------------------------
	// --------------------------------------------------
	
	{
		// Geometry pass
		rStorage->gBuffer->bind(FramebufferBinding::ReadWrite);
		RenderCommand::setViewport(0, 0, rStorage->gBuffer->getWidth(), rStorage->gBuffer->getHeight());
		RenderCommand::clearScreen();
		{
			while(!rStorage->modelRenderQueue.empty()) {
				auto model = rStorage->modelRenderQueue.front();

				if(model->getMesh() == nullptr) {
					CAPP_PRINT_ERROR("Failed to draw model: missing mesh!");
					continue;
				}
				if(model->getMaterial() == nullptr) {
					CAPP_PRINT_ERROR("Failed to draw model: missing material!");
					continue;
				}

				const auto mesh = model->getMesh();

				auto shader = rStorage->gBufferPass;
				model->getMaterial()->setShader(shader);

				shader->bind();
				shader->setUniform<Mat4>("uViewProjection", rStorage->perspectiveCamera.getViewProjection());
				shader->setUniform<Mat4>("uTransform", model->getTransform().getWorldTransform());
				shader->setUniform<Mat3>("uNormalMatrix", glm::mat3(glm::transpose(glm::inverse(model->getTransform().getWorldTransform()))));
				shader->setUniform<Float>("uGamma", rStorage->gamma);
				model->getMaterial()->apply();

				mesh->getVAO()->bind();
				if(mesh->getVAO()->getIndexBuffer() != nullptr) {
					RenderCommand::drawIndexed(mesh->getVAO());
				}
				else {
					RenderCommand::drawArray(mesh->getVAO());
				}

				rStorage->modelRenderQueue.pop_front();
			}
		}
		rStorage->gBuffer->unbind();

		RenderCommand::disableDepthTesting();
		RenderCommand::disableCulling();
		
		// Lighting pass
		rStorage->deferredComposite->bind();
		RenderCommand::setViewport(0, 0, rStorage->deferredComposite->getWidth(), rStorage->deferredComposite->getHeight());
		RenderCommand::clearScreen();
		{
			rStorage->gBuffer->getAttachment(AttachmentTarget::Colour0)->bind(0);
			rStorage->gBuffer->getAttachment(AttachmentTarget::Colour1)->bind(1);
			rStorage->gBuffer->getAttachment(AttachmentTarget::Colour2)->bind(2);
			rStorage->gBuffer->getAttachment(AttachmentTarget::Colour3)->bind(3);
			rStorage->gBuffer->getAttachment(AttachmentTarget::Colour4)->bind(4);
			rStorage->deferredLightingPass->bind();

			rStorage->deferredLightingPass->setUniform<Int>("uGBuffer.position",  0);
			rStorage->deferredLightingPass->setUniform<Int>("uGBuffer.normal",    1);
			rStorage->deferredLightingPass->setUniform<Int>("uGBuffer.albedo",    2);
			rStorage->deferredLightingPass->setUniform<Int>("uGBuffer.specRough", 3);
			rStorage->deferredLightingPass->setUniform<Int>("uGBuffer.emission",  4);

			rStorage->deferredLightingPass->setUniform<Vec3>("uCameraPosition", rStorage->perspectiveCamera.getPosition());

			rStorage->deferredLightingPass->setUniform<Int>("uNumPointLights", static_cast<int>(rStorage->activeLights.pointLights.size()));
			rStorage->deferredLightingPass->setUniform<Int>("uNumDirectionalLights", static_cast<int>(rStorage->activeLights.directionalLights.size()));
			rStorage->deferredLightingPass->setUniform<Int>("uNumSpotlights", static_cast<int>(rStorage->activeLights.spotlights.size()));

			rStorage->deferredLightingPass->setUniform<Vec3>("uAmbientColour", glm::vec3(0.2f, 0.4f, 0.6f));
			rStorage->deferredLightingPass->setUniform<Float>("uAmbientPower", 0.3f);

			for(unsigned i = 0; i < rStorage->activeLights.directionalLights.size(); ++i) {
				rStorage->deferredLightingPass->setUniform<Vec3>("uDirectionalLights[" + std::to_string(i) + "].direction", rStorage->activeLights.directionalLights[i]->getDirection());
				rStorage->deferredLightingPass->setUniform<Vec3>("uDirectionalLights[" + std::to_string(i) + "].colour", rStorage->activeLights.directionalLights[i]->getColour());
			}

			for(unsigned i = 0; i < rStorage->activeLights.pointLights.size(); ++i) {
				rStorage->deferredLightingPass->setUniform<Vec3>("uPointLights[" + std::to_string(i) + "].position", rStorage->activeLights.pointLights[i]->getPosition());
				rStorage->deferredLightingPass->setUniform<Vec3>("uPointLights[" + std::to_string(i) + "].colour", rStorage->activeLights.pointLights[i]->getColour());
				rStorage->deferredLightingPass->setUniform<Float>("uPointLights[" + std::to_string(i) + "].attenuation", rStorage->activeLights.pointLights[i]->getAttenuation());
			}

			for(unsigned i = 0; i < rStorage->activeLights.spotlights.size(); ++i) {
				rStorage->deferredLightingPass->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].position", rStorage->activeLights.spotlights[i]->getPosition());
				rStorage->deferredLightingPass->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].direction", rStorage->activeLights.spotlights[i]->getDirection());
				rStorage->deferredLightingPass->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].colour", rStorage->activeLights.spotlights[i]->getColour());
				rStorage->deferredLightingPass->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].attenuation", rStorage->activeLights.spotlights[i]->getAttenuation());
				rStorage->deferredLightingPass->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].innerCutoffAngle", glm::cos(glm::radians(rStorage->activeLights.spotlights[i]->getInnerCutoffAngle())));
				rStorage->deferredLightingPass->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].outerCutoffAngle", glm::cos(glm::radians(rStorage->activeLights.spotlights[i]->getOuterCutoffAngle())));
			}

			rStorage->fullscreenQuad->getVAO()->bind();
			RenderCommand::drawIndexed(rStorage->fullscreenQuad->getVAO());
		}
		rStorage->deferredComposite->unbind();

		// Copy depth info to deferred composite buffer to be able to render skybox
		{
			rStorage->gBuffer->bind(FramebufferBinding::ReadOnly);
			rStorage->deferredComposite->bind(FramebufferBinding::DrawOnly);
			Framebuffer::blitBufferData(
				{ 0, 0, rStorage->gBuffer->getWidth(), rStorage->gBuffer->getHeight() },
				{ 0, 0, rStorage->deferredComposite->getWidth(), rStorage->deferredComposite->getHeight() },
				ClearFlags::Depth, MagFilter::Nearest);
			rStorage->gBuffer->unbind();
			rStorage->deferredComposite->unbind();
		}

		// Skybox
		rStorage->deferredComposite->bind();
		{
			if(rStorage->activeSkybox != nullptr) {
				RenderCommand::disableCulling();
				RenderCommand::enableDepthTesting();
				RenderCommand::setDepthTestFunction(DepthTestFunction::LessThanOrEqual);
				RenderCommand::disableDepthMask();

				rStorage->skyboxShader->bind();
				const glm::mat4 viewProjection = rStorage->perspectiveCamera.getProjectionMatrix() * glm::mat4(glm::mat3(rStorage->perspectiveCamera.getViewMatrix()));
				rStorage->skyboxShader->setUniform<Mat4>("uViewProjection", viewProjection);

				rStorage->activeSkybox->bind(0);
				rStorage->skyboxShader->setUniform<Int>("uSkybox", 0);
				rStorage->skyboxShader->setUniform<Float>("uGamma", rStorage->gamma);

				rStorage->skyboxMesh->getVAO()->bind();
				RenderCommand::drawIndexed(rStorage->skyboxMesh->getVAO());

				RenderCommand::enableDepthMask();
				RenderCommand::setDepthTestFunction(DepthTestFunction::LessThan);
				RenderCommand::disableDepthTesting();
			}
		}
		rStorage->deferredComposite->unbind();
	}

	// TODO BLOCK
	{
		// TODO: HITBOX RENDERING
		#if 0
		if(Hitbox::shouldDraw()) {
			RenderCommand::disableCulling();
			RenderCommand::setDrawMode(DrawMode::Line);

			while(!rStorage->hitboxRenderQueue.empty()) {
				auto hitbox = rStorage->hitboxRenderQueue.front();

				rStorage->hitboxShader->bind();
				rStorage->hitboxShader->setUniform<Vec4>("uHitboxColour", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				rStorage->hitboxShader->setUniform<Mat4>("uTransform", hitbox->getTransform().getWorldTransform());
				rStorage->hitboxShader->setUniform<Mat4>("uViewProjection", rStorage->perspectiveCamera.getViewProjection());

				hitbox->getVAO()->bind();
				RenderCommand::drawIndexed(hitbox->getVAO());

				rStorage->hitboxRenderQueue.pop_front();
			}

			RenderCommand::setDrawMode(DrawMode::Fill);
			RenderCommand::enableCulling();
		}
		#endif

		// TODO: BLENDING AND FORWARD PASSES AFTER DEFERRED RENDERING
		#if 0
		// Forward rendering
		{
			rStorage->deferredComposite->bind();
			RenderCommand::setViewport(0, 0, rStorage->deferredComposite->getWidth(), rStorage->deferredComposite->getHeight());

			Shader* boundShader = nullptr;

			while(!rStorage->modelRenderQueue.empty()) {
				auto model = rStorage->modelRenderQueue.front();
				auto shader = model->getMaterial()->getShader();
				const auto mesh = model->getMesh();

				if(shader == nullptr) {
					CAPP_PRINT_WARNING("No shader bound, rendering with default shader");
					shader = rStorage->blinnPhongShader;
				}

				if(boundShader != shader) {
					boundShader = shader;
					boundShader->bind();

					shader->setUniform<Vec3>("uCameraPosition", rStorage->perspectiveCamera.getPosition());

					shader->setUniform<Int>("uNumPointLights", static_cast<int>(rStorage->activeLights.pointLights.size()));
					shader->setUniform<Int>("uNumDirectionalLights", static_cast<int>(rStorage->activeLights.directionalLights.size()));
					shader->setUniform<Int>("uNumSpotlights", static_cast<int>(rStorage->activeLights.spotlights.size()));

					shader->setUniform<Vec3>("uAmbientColour", glm::vec3(0.2f, 0.4f, 0.6f));
					shader->setUniform<Float>("uAmbientPower", 0.3f);

					for(unsigned i = 0; i < rStorage->activeLights.directionalLights.size(); ++i) {
						shader->setUniform<Vec3>("uDirectionalLights[" + std::to_string(i) + "].direction", rStorage->activeLights.directionalLights[i]->getDirection());
						shader->setUniform<Vec3>("uDirectionalLights[" + std::to_string(i) + "].colour", rStorage->activeLights.directionalLights[i]->getColour());
					}

					for(unsigned i = 0; i < rStorage->activeLights.pointLights.size(); ++i) {
						shader->setUniform<Vec3>("uPointLights[" + std::to_string(i) + "].position", rStorage->activeLights.pointLights[i]->getPosition());
						shader->setUniform<Vec3>("uPointLights[" + std::to_string(i) + "].colour", rStorage->activeLights.pointLights[i]->getColour());
						shader->setUniform<Float>("uPointLights[" + std::to_string(i) + "].attenuation", rStorage->activeLights.pointLights[i]->getAttenuation());
					}

					for(unsigned i = 0; i < rStorage->activeLights.spotlights.size(); ++i) {
						shader->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].position", rStorage->activeLights.spotlights[i]->getPosition());
						shader->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].direction", rStorage->activeLights.spotlights[i]->getDirection());
						shader->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].colour", rStorage->activeLights.spotlights[i]->getColour());
						shader->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].attenuation", rStorage->activeLights.spotlights[i]->getAttenuation());
						shader->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].innerCutoffAngle", glm::cos(glm::radians(rStorage->activeLights.spotlights[i]->getInnerCutoffAngle())));
						shader->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].outerCutoffAngle", glm::cos(glm::radians(rStorage->activeLights.spotlights[i]->getOuterCutoffAngle())));
					}
				}

				shader->setUniform<Mat4>("uViewProjection", rStorage->perspectiveCamera.getViewProjection());
				shader->setUniform<Mat4>("uTransform", model->getTransform().getWorldTransform());
				shader->setUniform<Mat3>("uNormalMatrix", glm::mat3(glm::transpose(glm::inverse(model->getTransform().getWorldTransform()))));
				model->getMaterial()->apply();

				mesh->getVAO()->bind();
				if(mesh->getVAO()->getIndexBuffer() != nullptr) { RenderCommand::drawIndexed(model->getMesh()->getVAO()); }
				else { RenderCommand::drawArray(model->getMesh()->getVAO()); }

				rStorage->modelRenderQueue.pop_front();
			}


			if(Hitbox::shouldDraw()) {
				RenderCommand::disableCulling();
				RenderCommand::setDrawMode(DrawMode::Line);

				while(!rStorage->hitboxRenderQueue.empty()) {
					auto hitbox = rStorage->hitboxRenderQueue.front();

					rStorage->hitboxShader->bind();
					rStorage->hitboxShader->setUniform<Vec4>("uHitboxColour", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					rStorage->hitboxShader->setUniform<Mat4>("uTransform", hitbox->getTransform().getWorldTransform());
					rStorage->hitboxShader->setUniform<Mat4>("uViewProjection", rStorage->perspectiveCamera.getViewProjection());

					hitbox->getVAO()->bind();
					RenderCommand::drawIndexed(hitbox->getVAO());

					rStorage->hitboxRenderQueue.pop_front();
				}

				RenderCommand::setDrawMode(DrawMode::Fill);
				RenderCommand::enableCulling();
			}

			if(rStorage->activeSkybox != nullptr) {
				RenderCommand::disableCulling();
				RenderCommand::setDepthTestFunction(DepthTestFunction::LessThanOrEqual);
				RenderCommand::disableDepthMask();

				rStorage->skyboxShader->bind();
				const glm::mat4 viewProjection = rStorage->perspectiveCamera.getProjectionMatrix() * glm::mat4(glm::mat3(rStorage->perspectiveCamera.getViewMatrix()));
				rStorage->skyboxShader->setUniform<Mat4>("uViewProjection", viewProjection);

				rStorage->activeSkybox->bind(0);
				rStorage->skyboxShader->setUniform<Int>("uSkybox", 0);

				rStorage->skyboxMesh->getVAO()->bind();
				RenderCommand::drawIndexed(rStorage->skyboxMesh->getVAO());

				RenderCommand::enableDepthMask();
				RenderCommand::setDepthTestFunction(DepthTestFunction::LessThan);
			}

			rStorage->deferredComposite->unbind();
		}
		#endif
	}
	// --------------------------------------------------
	// ----- Post-processing passes ---------------------
	// --------------------------------------------------

	Framebuffer* lastPass = rStorage->deferredComposite;
	{
		// Gamma correction
		{
			rStorage->gammaCorrection.buffer->validateFramebuffer();
			rStorage->gammaCorrection.buffer->bind();

			RenderCommand::setViewport(0, 0, rStorage->gammaCorrection.buffer->getWidth(), rStorage->gammaCorrection.buffer->getHeight());

			rStorage->gammaCorrection.shader->bind();
			lastPass->getAttachment(AttachmentTarget::Colour0)->bind(0);
			rStorage->gammaCorrection.shader->setUniform<Int>("uImage", 0);
			rStorage->gammaCorrection.shader->setUniform<Float>("uGamma", rStorage->gamma);
			rStorage->gammaCorrection.shader->setUniform<Vec2>("uScreenSize", glm::vec2(window->getWidth(), window->getHeight()));

			rStorage->fullscreenQuad->getVAO()->bind();
			RenderCommand::drawIndexed(rStorage->fullscreenQuad->getVAO());

			rStorage->gammaCorrection.buffer->unbind();
			lastPass = rStorage->gammaCorrection.buffer;
		}

		// Other post processing passes
		for(auto pass : postProcessing) {
			pass.buffer->validateFramebuffer();
			pass.buffer->bind();

			RenderCommand::setViewport(0, 0, pass.buffer->getWidth(), pass.buffer->getHeight());

			pass.shader->bind();
			lastPass->getAttachment(AttachmentTarget::Colour0)->bind(0);
			pass.shader->setUniform<Int>("uImage", 0);
			pass.shader->setUniform<Vec2>("uScreenSize", glm::vec2(window->getWidth(), window->getHeight()));

			rStorage->fullscreenQuad->getVAO()->bind();
			RenderCommand::drawIndexed(rStorage->fullscreenQuad->getVAO());

			pass.buffer->unbind();
			lastPass = pass.buffer;
		}
	}

	RenderCommand::setBlendFunction(SourceFactor::SourceAlpha, DestinationFactor::OneMinusSourceAlpha);

	// --------------------------------------------------
	// ----- Final image render -------------------------
	// --------------------------------------------------

	RenderCommand::setViewport(0, 0, lastPass->getWidth(), lastPass->getHeight());
	{
		rStorage->framebufferShader->bind();
		lastPass->getAttachment(AttachmentTarget::Colour0)->bind(0);
		rStorage->fullscreenQuad->getVAO()->bind();
		RenderCommand::drawIndexed(rStorage->fullscreenQuad->getVAO());
	}

	// --------------------------------------------------
	// ----- Unbind everything --------------------------
	// --------------------------------------------------

	{
		for(unsigned i = 0; i < 24; ++i) {
			Texture1D::unbind(i);
			Texture2D::unbind(i);
			Texture3D::unbind(i);
			TextureCubemap::unbind(i);
		}
		Shader::unbind();
		VertexArray::unbind();
	}
}
