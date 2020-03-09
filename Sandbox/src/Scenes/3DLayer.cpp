#include "Scenes/3DLayer.h"

void Layer3D::onPush() {

	// -----------------------------------------------------------------
	// ----- Floor plane -----------------------------------------------
	// -----------------------------------------------------------------

	{
		const auto planeMesh = Capp::MeshLibrary::loadMesh("Floor plane", "Assets/Cappuccino/Meshes/Cube.obj");

		unsigned blackTexture = 0x00000000;
		unsigned whiteTexture = 0xFFFFFFFF;
		auto planeMaterial = new Capp::Material;
		planeMaterial->setValue("diffuse", new Capp::Texture2D(1, 1, &whiteTexture));
		planeMaterial->setValue("specular", new Capp::Texture2D(1, 1, &blackTexture));
		planeMaterial->setValue("emission", new Capp::Texture2D(1, 1, &blackTexture));

		floorPlane = new Capp::Model(planeMesh, planeMaterial);
		floorPlane->setPosition(0.0f, -1.0f, 0.0f).setScale(20.0f, 0.1f, 20.0f);
	}
	
	// -----------------------------------------------------------------
	// ----- Game objects ----------------------------------------------
	// -----------------------------------------------------------------

	{
		captain1 = new Captain;
		captain1->setPosition(6.0f, 0.0f, 0.0f).setRotation(0.0, 90.0f, 0.0f);

		captain2 = new Captain;
		captain2->setPosition(-6.0f, 0.0f, 0.0f).setRotation(0.0, -90.0f, 0.0f);
	
		f16 = new F16;
	
		sentry1 = new SentryBot;
		sentry1->setPosition(0.0f, 0.0f, 6.0f).setRotation(0.0f, 180.0f, 0.0f);

		sentry2 = new SentryBot;
		sentry2->setPosition(0.0f, 0.0f, -6.0f);
	

		pointLight = new Capp::PointLight({ 0.0f, 1.0f, -2.0f }, { 0.8f, 0.2f, 0.3f }, 1.0f / 6.0f);
		dirLight = new Capp::DirectionalLight({ 0.0f, -1.0f, 0.0f }, { 0.1f, 0.4f, 0.15f });

		spotlight = new Capp::Spotlight;
		spotlight->setColour(0.3f, 0.2f, 0.7f).setAttenuation(1.0f / 20.0f).setInnerCutoffAngle(5.0f).setOuterCutoffAngle(22.5f);
	
		//Capp::Hitbox::setShouldDraw(true);
	}

	// -----------------------------------------------------------------
	// ----- Skybox ----------------------------------------------------
	// -----------------------------------------------------------------
	
	{
		const std::vector<std::string> filepaths = {
			"Assets/Textures/Skybox/corona/corona_lf.png",
			"Assets/Textures/Skybox/corona/corona_rt.png",
			"Assets/Textures/Skybox/corona/corona_up.png",
			"Assets/Textures/Skybox/corona/corona_dn.png",
			"Assets/Textures/Skybox/corona/corona_ft.png",
			"Assets/Textures/Skybox/corona/corona_bk.png"
		};

		skybox = new Capp::TextureCubemap(filepaths);
	}


	// -----------------------------------------------------------------
	// ----- LUTs ------------------------------------------------------
	// -----------------------------------------------------------------
	
	{
		//coolLUT = Capp::ResourceLoader::loadCUBEFile("Assets/LUTs/Cool-512.cube");
		//warmLUT = Capp::ResourceLoader::loadCUBEFile("Assets/LUTs/Warm-512.cube");
		//customLUT = Capp::ResourceLoader::loadCUBEFile("Assets/LUTs/Bleach-512.cube");
	}
	
	// -----------------------------------------------------------------
	// ----- Post-processing passes ------------------------------------
	// -----------------------------------------------------------------

	const auto window = Capp::Application::getInstance()->getWindow();
	const Capp::Attachment mainColour = { Capp::AttachmentType::Texture, Capp::InternalFormat::RGB8 };
	
	{
		grayscale.buffer = new Capp::Framebuffer(window->getWidth(), window->getHeight());
		grayscale.buffer->addAttachment(Capp::AttachmentTarget::Colour0, mainColour);
		grayscale.shader = Capp::ShaderLibrary::loadShader("Grayscale");
		grayscale.shader->attach("Assets/Cappuccino/Shaders/PostProcessing/PostProcessingShader.vert", Capp::ShaderStage::Vertex);
		grayscale.shader->attach("Assets/Shaders/PostProcessing/GrayscaleShader.frag", Capp::ShaderStage::Fragment);
		grayscale.shader->compile();

		inversion.buffer = new Capp::Framebuffer(window->getWidth(), window->getHeight());
		inversion.buffer->addAttachment(Capp::AttachmentTarget::Colour0, mainColour);
		inversion.shader = Capp::ShaderLibrary::loadShader("Inversion");
		inversion.shader->attach("Assets/Cappuccino/Shaders/PostProcessing/PostProcessingShader.vert", Capp::ShaderStage::Vertex);
		inversion.shader->attach("Assets/Shaders/PostProcessing/InversionShader.frag", Capp::ShaderStage::Fragment);
		inversion.shader->compile();

		colourGrading.buffer = new Capp::Framebuffer(window->getWidth(), window->getHeight());
		colourGrading.buffer->addAttachment(Capp::AttachmentTarget::Colour0, mainColour);
		colourGrading.shader = Capp::ShaderLibrary::loadShader("Colour Grading");
		colourGrading.shader->attach("Assets/Cappuccino/Shaders/PostProcessing/PostProcessingShader.vert", Capp::ShaderStage::Vertex);
		colourGrading.shader->attach("Assets/Shaders/PostProcessing/LUTColorGradingShader.frag", Capp::ShaderStage::Fragment);
		colourGrading.shader->compile();
	}
}

void Layer3D::onPop() {
	delete floorPlane;
	
	delete captain1;
	delete captain2;
	delete f16;
	delete sentry1;
	delete sentry2;
	
	delete dirLight;
	delete pointLight;
	delete spotlight;
	delete skybox;

	delete coolLUT;
	delete warmLUT;
	delete customLUT;
}

void Layer3D::update() {
	cameraController.update();

	Capp::Lights lights = { { dirLight }, { pointLight } };
	
	if(flashlight) {
		spotlight->setPosition(cameraController.getCamera().getPosition()).setDirection(cameraController.getCamera().getForward());
		lights.spotlights.push_back(spotlight);
	}

	#if 0
	const auto bpLut = Capp::ShaderLibrary::getShader("BlinnPhongModular");
	bpLut->bind();
	switch(lightSetting) {
		case 0:
			bpLut->setUniform<Capp::Bool>("uAmbientEnabled", false);
			bpLut->setUniform<Capp::Bool>("uDiffuseEnabled", false);
			bpLut->setUniform<Capp::Bool>("uSpecularEnabled", false);
			bpLut->setUniform<Capp::Bool>("uRimEnabled", false);
			break;
		case 1:
			bpLut->setUniform<Capp::Bool>("uAmbientEnabled", true);
			bpLut->setUniform<Capp::Bool>("uDiffuseEnabled", false);
			bpLut->setUniform<Capp::Bool>("uSpecularEnabled", false);
			bpLut->setUniform<Capp::Bool>("uRimEnabled", false);
			break;
		case 2:
			bpLut->setUniform<Capp::Bool>("uAmbientEnabled", false);
			bpLut->setUniform<Capp::Bool>("uDiffuseEnabled", false);
			bpLut->setUniform<Capp::Bool>("uSpecularEnabled", true);
			bpLut->setUniform<Capp::Bool>("uRimEnabled", false);
			break;
		case 3:
			bpLut->setUniform<Capp::Bool>("uAmbientEnabled", false);
			bpLut->setUniform<Capp::Bool>("uDiffuseEnabled", false);
			bpLut->setUniform<Capp::Bool>("uSpecularEnabled", true);
			bpLut->setUniform<Capp::Bool>("uRimEnabled", true);
			break;
		case 4:
			bpLut->setUniform<Capp::Bool>("uAmbientEnabled", true);
			bpLut->setUniform<Capp::Bool>("uDiffuseEnabled", true);
			bpLut->setUniform<Capp::Bool>("uSpecularEnabled", true);
			bpLut->setUniform<Capp::Bool>("uRimEnabled", true);
			break;
		default:
			break;
	}

	if(diffuseRampEnabled) {
		bpLut->setUniform<Capp::Bool>("uDiffuseRampEnabled", true);
		diffuseRamp->bind(6);
		bpLut->setUniform<Capp::Int>("uDiffuseRampTexture", 6);
	}
	else {
		bpLut->setUniform<Capp::Bool>("uDiffuseRampEnabled", false);
	}
	if(specularRampEnabled) {
		bpLut->setUniform<Capp::Bool>("uSpecularRampEnabled", true);
		specularRamp->bind(6);
		bpLut->setUniform<Capp::Int>("uSpecularRampTexture", 6);
	}
	else {
		bpLut->setUniform<Capp::Bool>("uSpecularRampEnabled", false);
	}
	#endif
	
	Capp::PostPasses postPasses;
	postPasses.reserve(5);
	if(isGray) {
		postPasses.push_back(grayscale);
	}
	if(isInverted) {
		postPasses.push_back(inversion);
	}
	if(lutSetting > 0) {
		colourGrading.shader->bind();
		switch(lutSetting) {
			case 1:
				coolLUT->bind(1);
				colourGrading.shader->setUniform<Capp::Vec3>("uLUTDimensions", coolLUT->getSize());
				break;
			case 2:
				warmLUT->bind(1);
				colourGrading.shader->setUniform<Capp::Vec3>("uLUTDimensions", warmLUT->getSize());
				break;
			case 3:
				customLUT->bind(1);
				colourGrading.shader->setUniform<Capp::Vec3>("uLUTDimensions", customLUT->getSize());
				break;
			default:
				break;
		}
		colourGrading.shader->setUniform<Capp::Int>("uLUTSlot", 1);
		postPasses.push_back(colourGrading);
	}

	if(isGammaCorrected) {
		Capp::Renderer::setGamma(gamma);
	}
	else {
		Capp::Renderer::setGamma(2.2f);
	}
	
	Capp::Renderer::start(cameraController.getCamera(), lights, skybox);
	{
		Capp::Renderer::addToQueue(floorPlane);
		Capp::Renderer::addToQueue(captain1);
		Capp::Renderer::addToQueue(captain2);
		Capp::Renderer::addToQueue(f16);
		Capp::Renderer::addToQueue(sentry1);
		Capp::Renderer::addToQueue(sentry2);
	}
	Capp::Renderer::finish(postPasses);
}

void Layer3D::drawImgui() {

	// --------------------------------------------------------------
	// ----- 3D layer settings --------------------------------------
	// --------------------------------------------------------------

	ImGui::Begin("3D Layer Settings");
	{
		ImGui::Checkbox("Grayscale", &isGray);
		ImGui::SameLine(); ImGui::Checkbox("Inversion", &isInverted);
		ImGui::Checkbox("Gamma correction", &isGammaCorrected);
		if(isGammaCorrected) {
			ImGui::DragFloat("Gamma", &gamma, 0.1f, 0.01f, 10.0f);
		}
		ImGui::NewLine();

		auto deferredLighting = Capp::ShaderLibrary::getShader("Deferred Lighting Default");
		if(ImGui::Button("Reload deferred lighting shader")) {
			deferredLighting->reload();
		}
	}
	ImGui::End();
	
	// --------------------------------------------------------------
	// ----- Colour grading settings --------------------------------
	// --------------------------------------------------------------
	
	ImGui::Begin("Colour grading");
	{
		ImGui::BeginGroup();
		{
			ImGui::RadioButton("None", &lutSetting, 0);
			ImGui::RadioButton("Cool", &lutSetting, 1);
			ImGui::RadioButton("Warm", &lutSetting, 2);
			ImGui::RadioButton("Bleach", &lutSetting, 3);
		}
		ImGui::EndGroup();
	}
	ImGui::End();

	// --------------------------------------------------------------
	// ----- Light settings -----------------------------------------
	// --------------------------------------------------------------

	// Directional light
	ImGui::Begin("Directional Light");
	{
		glm::vec3 dirLightDir = dirLight->getDirection();
		glm::vec3 dirLightColour = dirLight->getColour();

		ImGui::DragFloat3("Direction", glm::value_ptr(dirLightDir), 0.1f);
		ImGui::ColorEdit3("Colour", glm::value_ptr(dirLightColour));

		dirLight->setDirection(dirLightDir).setColour(dirLightColour);
	}
	ImGui::End();

	// Point light
	ImGui::Begin("Point Light");
	{
		glm::vec3 pointLightPos = pointLight->getPosition();
		glm::vec3 pointLightColour = pointLight->getColour();
		float pointLightRange = 1.0f / pointLight->getAttenuation();

		ImGui::DragFloat3("Position", glm::value_ptr(pointLightPos), 0.1f);
		ImGui::ColorEdit3("Colour", glm::value_ptr(pointLightColour));
		ImGui::DragFloat("Range", &pointLightRange, 1.0f, 1.0f, 100.0f);
	
		pointLight->setPosition(pointLightPos).setColour(pointLightColour).setAttenuation(1.0f / pointLightRange);
	}
	ImGui::End();

	// Flashlight (spotlight)
	ImGui::Begin("Flashlight");
	{
		glm::vec3 spotlightPos = spotlight->getPosition();
		glm::vec3 spotlightDir = spotlight->getDirection();
		glm::vec3 spotlightColour = spotlight->getColour();
		float spotlightRange = 1.0f / spotlight->getAttenuation();
		float innerRadius = spotlight->getInnerCutoffAngle();
		float outerRadius = spotlight->getOuterCutoffAngle();

		ImGui::DragFloat3("Position", glm::value_ptr(spotlightPos), 0.1f);
		ImGui::DragFloat3("Direction", glm::value_ptr(spotlightDir), 0.1f);
		ImGui::ColorEdit3("Colour", glm::value_ptr(spotlightColour));
		ImGui::DragFloat("Range", &spotlightRange, 1.0f, 1.0f, 100.0f);
		ImGui::DragFloat("Inner cutoff angle", &innerRadius, 0.1f, 0.1f, 45.0f);
		ImGui::DragFloat("Outer cutoff angle", &outerRadius, 0.1f, 0.1f, 45.0f);
	
		spotlight->setPosition(spotlightPos).setDirection(spotlightDir).setColour(spotlightColour).setAttenuation(1.0f / spotlightRange).setInnerCutoffAngle(innerRadius).setOuterCutoffAngle(outerRadius);
	}
	ImGui::End();
}

void Layer3D::onEvent(Capp::Event& e) {
	Capp::EventDispatcher d(e);
	d.dispatchEventType<Capp::KeyPressedEvent>(BIND_EVENT_FN(Layer3D::onKeyPressed));
	
	cameraController.onEvent(e);
}

bool Layer3D::onKeyPressed(Capp::KeyPressedEvent& e) {
	if(e.compareKey(Capp::KeyCode::F)) {
		flashlight ^= 1;
	}

	return false;
}

