#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Renderer.h"

#include "Cappuccino/Core/Application.h"
#include "Cappuccino/Rendering/Framebuffer.h"
#include "Cappuccino/Rendering/RenderCommand.h"
#include "Cappuccino/Rendering/3D/TextureCubemap.h"

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

	std::map<unsigned, std::pair<Shader*, VertexArray*>> renderList;
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
		rendererStorage->defaultShader = new Shader("3D Default", "Assets/Cappuccino/Shaders/BlinnPhongShader.vert", "Assets/Cappuccino/Shaders/BlinnPhongShader.frag");
		rendererStorage->hitboxShader = new Shader("Hitbox Default", "Assets/Cappuccino/Shaders/HitboxShader.vert", "Assets/Cappuccino/Shaders/HitboxShader.frag");
		rendererStorage->skyboxShader = new Shader("Skybox Default", "Assets/Cappuccino/Shaders/CubemapShader.vert", "Assets/Cappuccino/Shaders/CubemapShader.frag");
		rendererStorage->mainBufferShader = new Shader("Framebuffer Default", "Assets/Cappuccino/Shaders/FramebufferShader.vert", "Assets/Cappuccino/Shaders/FramebufferShader.frag");
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
			"Assets/Cappuccino/Textures/Skybox/graycloud_lf.jpg",
			"Assets/Cappuccino/Textures/Skybox/graycloud_rt.jpg",
			"Assets/Cappuccino/Textures/Skybox/graycloud_dn.jpg",
			"Assets/Cappuccino/Textures/Skybox/graycloud_up.jpg",
			"Assets/Cappuccino/Textures/Skybox/graycloud_ft.jpg",
			"Assets/Cappuccino/Textures/Skybox/graycloud_bk.jpg"
		};

		rendererStorage->defaultSkybox = new TextureCubemap(filepaths);
	}
	// --------------------------------------------------
	// ----- Main framebuffer ---------------------------
	// --------------------------------------------------

	{
		rendererStorage->mainBuffer = new Framebuffer(window->getWidth(), window->getHeight());
		rendererStorage->mainBuffer->setName("Main Framebuffer");
		const Attachment mainColour = { AttachmentType::Texture, AttachmentFormat::RGB8 };
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
	
	// --------------------------------------------------
	// ----- Clear colour -------------------------------
	// --------------------------------------------------
	
	RenderCommand::setClearColour(0.8f, 0.2f, 0.7f, 1.0f);
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

void Renderer::start(const PerspectiveCamera& camera, const Lights& lights, TextureCubemap* skybox) {
	rendererStorage->perspectiveCamera = camera;
	rendererStorage->activeLights.directionalLights = lights.directionalLights;
	rendererStorage->activeLights.pointLights = lights.pointLights;
	rendererStorage->activeLights.spotlights = lights.spotlights;
	rendererStorage->activeSkybox = skybox;
}

void Renderer::addToRenderList(VertexArray* vertexArray, Shader* shader) {
	rendererStorage->renderList[shader->getRendererID()] = { shader, vertexArray };
}

void Renderer::addToRenderList(VertexArray* vertexArray, Material* material) {
	if(material->getShader() == nullptr) {
		material->setShader(rendererStorage->defaultShader);
	}
	
	material->apply();
	addToRenderList(vertexArray, material->getShader());
}

void Renderer::addToRenderList(Mesh* mesh, Shader* shader) {
	addToRenderList(mesh->getVAO(), shader);
}

void Renderer::addToRenderList(Mesh* mesh, Material* material) {
	material->apply();
	addToRenderList(mesh, material->getShader());
}

void Renderer::addToRenderList(Model* model) {
	model->getMaterial()->getShader()->bind();
	model->getMaterial()->getShader()->setUniform("uTransform", model->getTransform().getWorldTransform());
	model->getMaterial()->getShader()->setUniform("uNormalMatrix", glm::mat3(glm::transpose(glm::inverse(model->getTransform().getWorldTransform()))));
	addToRenderList(model->getMesh(), model->getMaterial());
}

void Renderer::addToRenderList(Hitbox* hitbox) {
	if(!Hitbox::shouldDraw()) {
		return;
	}

	RenderCommand::disableCulling();
	RenderCommand::setDrawMode(DrawMode::Line);

	rendererStorage->hitboxShader->bind();
	rendererStorage->hitboxShader->setUniform("uHitboxColour", { 1.0f, 0.0f, 0.0f, 1.0f });
	rendererStorage->hitboxShader->setUniform("uTransform", hitbox->getTransform().getWorldTransform());
	rendererStorage->hitboxShader->setUniform("uViewProjection", rendererStorage->perspectiveCamera.getViewProjection());

	hitbox->getVAO()->bind();
	RenderCommand::drawIndexed(hitbox->getVAO());

	RenderCommand::setDrawMode(DrawMode::Fill);
	RenderCommand::enableCulling();
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

void Renderer::finish() {

	rendererStorage->mainBuffer->bind();
	RenderCommand::setViewport(0, 0, rendererStorage->mainBuffer->getWidth(), rendererStorage->mainBuffer->getHeight());
	RenderCommand::clearScreen(ClearFlags::NoStencil);
	RenderCommand::enableDepthTesting();
	RenderCommand::enableCulling();

	// Render scene
	{
		Shader* boundShader = nullptr;

		for(auto& object : rendererStorage->renderList) {
			Shader* objectShader = object.second.first;
			VertexArray* objectVao = object.second.second;
		
			if(objectShader == nullptr) {
				CAPP_PRINT_WARNING("No shader bound, rendering with default shader");
				objectShader = rendererStorage->defaultShader;
			}

			if(boundShader != objectShader) {
				//CAPP_PRINT("Rendering with shader \"{0}\" (ID: {1})", objectShader->getName(), objectShader->getRendererID());
				boundShader = objectShader;
				boundShader->bind();
				objectShader->setUniform("uCameraPosition", rendererStorage->perspectiveCamera.getPosition());
				objectShader->setUniform("uViewProjection", rendererStorage->perspectiveCamera.getViewProjection());
				objectShader->setUniform("uNumPointLights", static_cast<int>(rendererStorage->activeLights.pointLights.size()));
				objectShader->setUniform("uNumDirectionalLights", static_cast<int>(rendererStorage->activeLights.directionalLights.size()));
				objectShader->setUniform("uNumSpotlights", static_cast<int>(rendererStorage->activeLights.spotlights.size()));

				objectShader->setUniform("uAmbientColour", { 0.2f, 0.4f, 0.8f });
				objectShader->setUniform("uAmbientPower", 0.3f);

				for(unsigned i = 0; i < rendererStorage->activeLights.directionalLights.size(); ++i) {
					objectShader->setUniform("uDirectionalLights[" + std::to_string(i) + "].direction", rendererStorage->activeLights.directionalLights[i]->getDirection());
					objectShader->setUniform("uDirectionalLights[" + std::to_string(i) + "].colour", rendererStorage->activeLights.directionalLights[i]->getColour());
				}

				for(unsigned i = 0; i < rendererStorage->activeLights.pointLights.size(); ++i) {
					objectShader->setUniform("uPointLights[" + std::to_string(i) + "].position", rendererStorage->activeLights.pointLights[i]->getPosition());
					objectShader->setUniform("uPointLights[" + std::to_string(i) + "].colour", rendererStorage->activeLights.pointLights[i]->getColour());
					objectShader->setUniform("uPointLights[" + std::to_string(i) + "].attenuation", rendererStorage->activeLights.pointLights[i]->getAttenuation());
				}

				for(unsigned i = 0; i < rendererStorage->activeLights.spotlights.size(); ++i) {
					objectShader->setUniform("uSpotlights[" + std::to_string(i) + "].position", rendererStorage->activeLights.spotlights[i]->getPosition());
					objectShader->setUniform("uSpotlights[" + std::to_string(i) + "].direction", rendererStorage->activeLights.spotlights[i]->getDirection());
					objectShader->setUniform("uSpotlights[" + std::to_string(i) + "].colour", rendererStorage->activeLights.spotlights[i]->getColour());
					objectShader->setUniform("uSpotlights[" + std::to_string(i) + "].attenuation", rendererStorage->activeLights.spotlights[i]->getAttenuation());
					objectShader->setUniform("uSpotlights[" + std::to_string(i) + "].innerCutoffAngle", glm::cos(glm::radians(rendererStorage->activeLights.spotlights[i]->getInnerCutoffAngle())));
					objectShader->setUniform("uSpotlights[" + std::to_string(i) + "].outerCutoffAngle", glm::cos(glm::radians(rendererStorage->activeLights.spotlights[i]->getOuterCutoffAngle())));
				}
			
			}
		
			objectVao->bind();
			if(objectVao->getIndexBuffer() != nullptr) {
				RenderCommand::drawIndexed(objectVao);
			}
			else {
				RenderCommand::drawArray(objectVao);
			}
		}
	
		RenderCommand::disableCulling();
		RenderCommand::setDepthTestFunction(DepthTestFunction::LessThanOrEqual);
		RenderCommand::disableDepthMask();

		rendererStorage->skyboxShader->bind();
		const glm::mat4 viewProjection = rendererStorage->perspectiveCamera.getProjectionMatrix() * glm::mat4(glm::mat3(rendererStorage->perspectiveCamera.getViewMatrix()));
		rendererStorage->skyboxShader->setUniform("uViewProjection", viewProjection);

		rendererStorage->activeSkybox->bind(0);
		rendererStorage->skyboxShader->setUniform("uSkybox", 0);

		rendererStorage->skyboxMesh->getVAO()->bind();
		RenderCommand::drawIndexed(rendererStorage->skyboxMesh->getVAO());

		RenderCommand::enableDepthMask();
		RenderCommand::setDepthTestFunction(DepthTestFunction::LessThan);
	}

	rendererStorage->mainBuffer->unbind();
	RenderCommand::disableDepthTesting();
	RenderCommand::clearScreen(ClearFlags::Colour);

	rendererStorage->mainBuffer->bind(FramebufferBinding::ReadOnly);
	Framebuffer::blit({0, 0, rendererStorage->mainBuffer->getWidth(), rendererStorage->mainBuffer->getHeight()}, 
		{0, 0, Application::getInstance()->getWindow()->getWidth(), Application::getInstance()->getWindow()->getHeight()},
		ClearFlags::All, MagFilter::Nearest);
	rendererStorage->mainBuffer->unbind();
	
	for(unsigned i = 0; i < 24; ++i) {
		Texture2D::unbind(i);
	}
	Shader::unbind();
	TextureCubemap::unbind();
	VertexArray::unbind();
}