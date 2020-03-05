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
	Shader* gBufferPass          = nullptr;
	Shader* deferredLightingPass = nullptr;

	Lights defaultLights;
	Lights activeLights;

	Mesh* skyboxMesh     = nullptr;
	Mesh* fullscreenQuad = nullptr;

	TextureCubemap* defaultSkybox = nullptr;
	TextureCubemap* activeSkybox  = nullptr;

	Framebuffer* deferredComposite = nullptr;
	Framebuffer* gBuffer           = nullptr;

	std::deque<Model*> modelRenderQueue;
	std::deque<Hitbox*> hitboxRenderQueue;
};

static RendererStorage* rendererStorage;

void Renderer::init() {
	rendererStorage   = new RendererStorage;
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
		rendererStorage->blinnPhongShader = new Shader("Blinn Phong Default");
		rendererStorage->blinnPhongShader->attach("Assets/Cappuccino/Shaders/SimpleGeometry.vert", ShaderStage::Vertex);
		rendererStorage->blinnPhongShader->attach("Assets/Cappuccino/Shaders/BlinnPhongShader.frag", ShaderStage::Fragment);
		rendererStorage->blinnPhongShader->compile();

		rendererStorage->hitboxShader = new Shader("Hitbox Default");
		rendererStorage->hitboxShader->attach("Assets/Cappuccino/Shaders/HitboxShader.vert", ShaderStage::Vertex);
		rendererStorage->hitboxShader->attach("Assets/Cappuccino/Shaders/HitboxShader.frag", ShaderStage::Fragment);
		rendererStorage->hitboxShader->compile();

		rendererStorage->skyboxShader = new Shader("Skybox Default");
		rendererStorage->skyboxShader->attach("Assets/Cappuccino/Shaders/CubemapShader.vert", ShaderStage::Vertex);
		rendererStorage->skyboxShader->attach("Assets/Cappuccino/Shaders/CubemapShader.frag", ShaderStage::Fragment);
		rendererStorage->skyboxShader->compile();

		rendererStorage->framebufferShader = new Shader("Framebuffer Default");
		rendererStorage->framebufferShader->attach("Assets/Cappuccino/Shaders/FramebufferShader.vert", ShaderStage::Vertex);
		rendererStorage->framebufferShader->attach("Assets/Cappuccino/Shaders/FramebufferShader.frag", ShaderStage::Fragment);
		rendererStorage->framebufferShader->compile();

		rendererStorage->gBufferPass = new Shader("Deferred GBuffer Default");
		rendererStorage->gBufferPass->attach("Assets/Cappuccino/Shaders/SimpleGeometry.vert", ShaderStage::Vertex);
		rendererStorage->gBufferPass->attach("Assets/Cappuccino/Shaders/GBufferPass.frag", ShaderStage::Fragment);
		rendererStorage->gBufferPass->compile();

		rendererStorage->deferredLightingPass = new Shader("Deferred Lighting Default");
		rendererStorage->deferredLightingPass->attach("Assets/Cappuccino/Shaders/DeferredLightingShader.vert", ShaderStage::Vertex);
		rendererStorage->deferredLightingPass->attach("Assets/Cappuccino/Shaders/DeferredLightingShader.frag", ShaderStage::Fragment);
		rendererStorage->deferredLightingPass->compile();
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

		rendererStorage->activeLights.directionalLights.reserve(MAX_DIR_LIGHTS);
		rendererStorage->activeLights.pointLights.reserve(MAX_POINT_LIGHTS);
		rendererStorage->activeLights.spotlights.reserve(MAX_SPOTLIGHTS);
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

	// --------------------------------------------------
	// ----- GBuffer ------------------------------------
	// --------------------------------------------------

	{
		rendererStorage->gBuffer = new Framebuffer(window->getWidth(), window->getHeight());
		rendererStorage->gBuffer->setName("GBuffer");
		const Attachment position     = { AttachmentType::Texture, InternalFormat::RGBA32F, { WrapMode::Repeat, MinFilter::Nearest, MagFilter::Nearest, Anisotropy::Aniso1x } };
		const Attachment normal       = { AttachmentType::Texture, InternalFormat::RGBA16F, { WrapMode::Repeat, MinFilter::Nearest, MagFilter::Nearest, Anisotropy::Aniso1x } };
		const Attachment albedo       = { AttachmentType::Texture, InternalFormat::RGBA8,   { WrapMode::Repeat, MinFilter::Nearest, MagFilter::Nearest, Anisotropy::Aniso1x } };
		const Attachment specRough    = { AttachmentType::Texture, InternalFormat::RGBA8,   { WrapMode::Repeat, MinFilter::Nearest, MagFilter::Nearest, Anisotropy::Aniso1x } };
		const Attachment emission     = { AttachmentType::Texture, InternalFormat::RGBA8,   { WrapMode::Repeat, MinFilter::Nearest, MagFilter::Nearest, Anisotropy::Aniso1x } };
		const Attachment depthStencil = { AttachmentType::RenderBuffer, InternalFormat::Depth24_Stencil8 };

		rendererStorage->gBuffer->addAttachment(AttachmentTarget::Colour0, position);
		rendererStorage->gBuffer->addAttachment(AttachmentTarget::Colour1, normal);
		rendererStorage->gBuffer->addAttachment(AttachmentTarget::Colour2, albedo);
		rendererStorage->gBuffer->addAttachment(AttachmentTarget::Colour3, specRough);
		rendererStorage->gBuffer->addAttachment(AttachmentTarget::Colour4, emission);
		rendererStorage->gBuffer->addAttachment(AttachmentTarget::DepthStencil, depthStencil);
	}

	// --------------------------------------------------
	// ----- Deferred Composite Framebuffer -------------
	// --------------------------------------------------

	{
		rendererStorage->deferredComposite = new Framebuffer(window->getWidth(), window->getHeight());
		rendererStorage->deferredComposite->setName("Deferred Composite Framebuffer");
		const Attachment mainColour   = { AttachmentType::Texture, InternalFormat::RGBA8 };
		const Attachment depthStencil = { AttachmentType::RenderBuffer, InternalFormat::Depth24_Stencil8 };
		rendererStorage->deferredComposite->addAttachment(AttachmentTarget::Colour0, mainColour);
		rendererStorage->deferredComposite->addAttachment(AttachmentTarget::DepthStencil, depthStencil);
	}

}

void Renderer::shutdown() {
	delete rendererStorage;
}

void Renderer::onWindowResized(const unsigned width, const unsigned height) {
	rendererStorage->defaultCamera.setProjection(60.0f, width, height);
	rendererStorage->deferredComposite->resize(width, height);
}

void Renderer::start() {
	start(rendererStorage->defaultCamera, rendererStorage->defaultLights, rendererStorage->defaultSkybox);
}

void Renderer::start(const PerspectiveCamera& camera, const Lights& lights, const Skybox& skybox) {
	rendererStorage->perspectiveCamera              = camera;
	rendererStorage->activeLights.directionalLights = lights.directionalLights;
	rendererStorage->activeLights.pointLights       = lights.pointLights;
	rendererStorage->activeLights.spotlights        = lights.spotlights;
	rendererStorage->activeSkybox                   = skybox.value_or(nullptr);
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

	addToQueue(vertexArray, material->getShader());
}

void Renderer::addToQueue(Mesh* mesh, Shader* shader) {
	addToQueue(mesh->getVAO(), shader);
}

void Renderer::addToQueue(Mesh* mesh, Material* material) {
	addToQueue(mesh->getVAO(), material->getShader());
}

void Renderer::addToQueue(Model* model) {
	rendererStorage->modelRenderQueue.push_back(model);
}

void Renderer::addToQueue(Hitbox* hitbox) {
	rendererStorage->hitboxRenderQueue.push_back(hitbox);
}

void Renderer::addToQueue(const RigidBody& rigidBody) {
	if(!Hitbox::shouldDraw()) {
		return;
	}
	for(auto hitbox : rigidBody) {
		addToQueue(hitbox);
	}
}

void Renderer::addToQueue(GameObject* gameObject) {
	for(auto model : gameObject->getModels()) {
		addToQueue(model);
	}
	if(gameObject->isVisible()) {
		addToQueue(gameObject->getRigidBody());
	}
}

void Renderer::finish(const PostPasses& postProcessing) {

	// Sort by shader ID (to avoid rebinding the same shader)
	std::sort(rendererStorage->modelRenderQueue.begin(), rendererStorage->modelRenderQueue.end(), [](Model* first, const Model* second)-> bool {
		if(second->getMaterial() == nullptr || second->getMesh() == nullptr) {
			return false;
		}
		if(first->getMaterial() == nullptr || first->getMesh() == nullptr) {
			return true;
		}
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

	#if 0
	// Forward rendering
	{
		rendererStorage->deferredComposite->bind();
		RenderCommand::setViewport(0, 0, rendererStorage->deferredComposite->getWidth(), rendererStorage->deferredComposite->getHeight());
		
		Shader* boundShader = nullptr;

		while(!rendererStorage->modelRenderQueue.empty()) {
			auto model = rendererStorage->modelRenderQueue.front();
			auto shader = model->getMaterial()->getShader();
			const auto mesh = model->getMesh();

			if(shader == nullptr) {
				CAPP_PRINT_WARNING("No shader bound, rendering with default shader");
				shader = rendererStorage->blinnPhongShader;
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

			mesh->getVAO()->bind();
			if(mesh->getVAO()->getIndexBuffer() != nullptr) { RenderCommand::drawIndexed(model->getMesh()->getVAO()); }
			else { RenderCommand::drawArray(model->getMesh()->getVAO()); }

			rendererStorage->modelRenderQueue.pop_front();
		}


		if(Hitbox::shouldDraw()) {
			RenderCommand::disableCulling();
			RenderCommand::setDrawMode(DrawMode::Line);

			while(!rendererStorage->hitboxRenderQueue.empty()) {
				auto hitbox = rendererStorage->hitboxRenderQueue.front();

				rendererStorage->hitboxShader->bind();
				rendererStorage->hitboxShader->setUniform<Vec4>("uHitboxColour", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				rendererStorage->hitboxShader->setUniform<Mat4>("uTransform", hitbox->getTransform().getWorldTransform());
				rendererStorage->hitboxShader->setUniform<Mat4>("uViewProjection", rendererStorage->perspectiveCamera.getViewProjection());

				hitbox->getVAO()->bind();
				RenderCommand::drawIndexed(hitbox->getVAO());

				rendererStorage->hitboxRenderQueue.pop_front();
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

		rendererStorage->deferredComposite->unbind();
	}
	#endif

	// Deferred rendering
	{
		// Geometry pass
		rendererStorage->gBuffer->bind(FramebufferBinding::ReadWrite);
		RenderCommand::clearScreen();
		{
			while(!rendererStorage->modelRenderQueue.empty()) {
				auto model = rendererStorage->modelRenderQueue.front();

				if(model->getMesh() == nullptr) {
					CAPP_PRINT_ERROR("Failed to draw model: missing mesh!");
					continue;
				}
				if(model->getMaterial() == nullptr) {
					CAPP_PRINT_ERROR("Failed to draw model: missing material!");
					continue;
				}

				const auto mesh = model->getMesh();

				auto shader = rendererStorage->gBufferPass;
				model->getMaterial()->setShader(shader);

				shader->bind();
				shader->setUniform<Mat4>("uViewProjection", rendererStorage->perspectiveCamera.getViewProjection());
				shader->setUniform<Mat4>("uTransform", model->getTransform().getWorldTransform());
				shader->setUniform<Mat3>("uNormalMatrix", glm::mat3(glm::transpose(glm::inverse(model->getTransform().getWorldTransform()))));
				model->getMaterial()->apply();

				mesh->getVAO()->bind();
				if(mesh->getVAO()->getIndexBuffer() != nullptr) {
					RenderCommand::drawIndexed(model->getMesh()->getVAO());
				}
				else {
					RenderCommand::drawArray(model->getMesh()->getVAO());
				}

				rendererStorage->modelRenderQueue.pop_front();
			}

			#if 0
			if(Hitbox::shouldDraw()) {
				RenderCommand::disableCulling();
				RenderCommand::setDrawMode(DrawMode::Line);

				while(!rendererStorage->hitboxRenderQueue.empty()) {
					auto hitbox = rendererStorage->hitboxRenderQueue.front();

					rendererStorage->hitboxShader->bind();
					rendererStorage->hitboxShader->setUniform<Vec4>("uHitboxColour", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					rendererStorage->hitboxShader->setUniform<Mat4>("uTransform", hitbox->getTransform().getWorldTransform());
					rendererStorage->hitboxShader->setUniform<Mat4>("uViewProjection", rendererStorage->perspectiveCamera.getViewProjection());

					hitbox->getVAO()->bind();
					RenderCommand::drawIndexed(hitbox->getVAO());

					rendererStorage->hitboxRenderQueue.pop_front();
				}

				RenderCommand::setDrawMode(DrawMode::Fill);
				RenderCommand::enableCulling();
			}
			#endif
		}
		rendererStorage->gBuffer->unbind();

		// Lighting pass
		rendererStorage->deferredComposite->bind();
		RenderCommand::setViewport(0, 0, rendererStorage->deferredComposite->getWidth(), rendererStorage->deferredComposite->getHeight());
		RenderCommand::clearScreen();
		{
			rendererStorage->gBuffer->getAttachment(AttachmentTarget::Colour0)->bind(0);
			rendererStorage->gBuffer->getAttachment(AttachmentTarget::Colour1)->bind(1);
			rendererStorage->gBuffer->getAttachment(AttachmentTarget::Colour2)->bind(2);
			rendererStorage->gBuffer->getAttachment(AttachmentTarget::Colour3)->bind(3);
			rendererStorage->gBuffer->getAttachment(AttachmentTarget::Colour4)->bind(4);
			rendererStorage->deferredLightingPass->bind();

			rendererStorage->deferredLightingPass->setUniform<Int>("uGBuffer.position",  0);
			rendererStorage->deferredLightingPass->setUniform<Int>("uGBuffer.normal",    1);
			rendererStorage->deferredLightingPass->setUniform<Int>("uGBuffer.albedo",    2);
			rendererStorage->deferredLightingPass->setUniform<Int>("uGBuffer.specRough", 3);
			rendererStorage->deferredLightingPass->setUniform<Int>("uGBuffer.emission",  4);

			rendererStorage->deferredLightingPass->setUniform<Vec3>("uCameraPosition", rendererStorage->perspectiveCamera.getPosition());

			rendererStorage->deferredLightingPass->setUniform<Int>("uNumPointLights", static_cast<int>(rendererStorage->activeLights.pointLights.size()));
			rendererStorage->deferredLightingPass->setUniform<Int>("uNumDirectionalLights", static_cast<int>(rendererStorage->activeLights.directionalLights.size()));
			rendererStorage->deferredLightingPass->setUniform<Int>("uNumSpotlights", static_cast<int>(rendererStorage->activeLights.spotlights.size()));

			rendererStorage->deferredLightingPass->setUniform<Vec3>("uAmbientColour", glm::vec3(0.2f, 0.4f, 0.6f));
			rendererStorage->deferredLightingPass->setUniform<Float>("uAmbientPower", 0.3f);

			for(unsigned i = 0; i < rendererStorage->activeLights.directionalLights.size(); ++i) {
				rendererStorage->deferredLightingPass->setUniform<Vec3>("uDirectionalLights[" + std::to_string(i) + "].direction", rendererStorage->activeLights.directionalLights[i]->getDirection());
				rendererStorage->deferredLightingPass->setUniform<Vec3>("uDirectionalLights[" + std::to_string(i) + "].colour", rendererStorage->activeLights.directionalLights[i]->getColour());
			}

			for(unsigned i = 0; i < rendererStorage->activeLights.pointLights.size(); ++i) {
				rendererStorage->deferredLightingPass->setUniform<Vec3>("uPointLights[" + std::to_string(i) + "].position", rendererStorage->activeLights.pointLights[i]->getPosition());
				rendererStorage->deferredLightingPass->setUniform<Vec3>("uPointLights[" + std::to_string(i) + "].colour", rendererStorage->activeLights.pointLights[i]->getColour());
				rendererStorage->deferredLightingPass->setUniform<Float>("uPointLights[" + std::to_string(i) + "].attenuation", rendererStorage->activeLights.pointLights[i]->getAttenuation());
			}

			for(unsigned i = 0; i < rendererStorage->activeLights.spotlights.size(); ++i) {
				rendererStorage->deferredLightingPass->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].position", rendererStorage->activeLights.spotlights[i]->getPosition());
				rendererStorage->deferredLightingPass->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].direction", rendererStorage->activeLights.spotlights[i]->getDirection());
				rendererStorage->deferredLightingPass->setUniform<Vec3>("uSpotlights[" + std::to_string(i) + "].colour", rendererStorage->activeLights.spotlights[i]->getColour());
				rendererStorage->deferredLightingPass->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].attenuation", rendererStorage->activeLights.spotlights[i]->getAttenuation());
				rendererStorage->deferredLightingPass->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].innerCutoffAngle", glm::cos(glm::radians(rendererStorage->activeLights.spotlights[i]->getInnerCutoffAngle())));
				rendererStorage->deferredLightingPass->setUniform<Float>("uSpotlights[" + std::to_string(i) + "].outerCutoffAngle", glm::cos(glm::radians(rendererStorage->activeLights.spotlights[i]->getOuterCutoffAngle())));
			}

			rendererStorage->fullscreenQuad->getVAO()->bind();
			RenderCommand::drawIndexed(rendererStorage->fullscreenQuad->getVAO());
		}
		rendererStorage->deferredComposite->unbind();
	}

	// TODO: SKYBOX

	#if 0
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
	#endif

	// TODO: HITBOX RENDERING
	// TODO: BLENDING AND FORWARD PASSES AFTER DEFERRED RENDERING

	#if 0
	RenderCommand::disableDepthTesting();
	const auto window = Application::getInstance()->getWindow();

	// Post processing passes
	Framebuffer* lastPass = rendererStorage->deferredComposite;
	for(auto pass : postProcessing) {
		pass.buffer->validateFramebuffer();
		pass.buffer->bind();

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

	RenderCommand::setViewport(0, 0, lastPass->getWidth(), lastPass->getHeight());
	rendererStorage->framebufferShader->bind();
	lastPass->getAttachment(AttachmentTarget::Colour0)->bind(0);
	rendererStorage->fullscreenQuad->getVAO()->bind();
	RenderCommand::drawIndexed(rendererStorage->fullscreenQuad->getVAO());

	rendererStorage->gBuffer->bind(FramebufferBinding::ReadOnly);
	Framebuffer::blitBufferData(
		{ 0, 0, rendererStorage->gBuffer->getWidth(), rendererStorage->gBuffer->getHeight() },
		{ 0, 0, window->getWidth(), window->getHeight() },
		ClearFlags::Depth, MagFilter::Nearest);
	rendererStorage->gBuffer->unbind();
	#endif

	for(unsigned i = 0; i < 24; ++i) {
		Texture1D::unbind(i);
		Texture2D::unbind(i);
		Texture3D::unbind(i);
		TextureCubemap::unbind(i);
	}
	Shader::unbind();
	VertexArray::unbind();
}
