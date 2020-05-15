#pragma once
#include <Cappuccino.h>

#include "Objects/CameraController.h"
#include "Objects/Captain.h"
#include "Objects/F16.h"
#include "Objects/SentryBot.h"

class Layer3D : public Capp::Layer {
public:

	Layer3D() : Layer("3D Layer") {}
	~Layer3D() = default;

	void onPush() override;
	void onPop() override;

	void update() override;

	void onEvent(Capp::Event& e) override;

	void drawImgui() override;

private:

	bool onKeyPressed(Capp::KeyPressedEvent& e);

	CameraController cameraController;
	bool flashlight = true;

	Ref<Capp::Model> floor   = nullptr;
	Ref<Capp::Model> wall1   = nullptr;
	Ref<Capp::Model> wall2   = nullptr;
	Ref<Capp::Model> wall3   = nullptr;
	Ref<Capp::Model> wall4   = nullptr;
	Ref<Capp::Model> ceiling = nullptr;

	
	Ref<Capp::Model> pointLightCube = nullptr;
	Ref<Capp::Model> spotlightCube = nullptr;
	
	Ref<Captain> captain1  = nullptr;
	Ref<Captain> captain2  = nullptr;
	Ref<F16> f16           = nullptr;
	Ref<SentryBot> sentry1 = nullptr;
	Ref<SentryBot> sentry2 = nullptr;
	
	Ref<Capp::PointLight> pointLight     = nullptr;
	Ref<Capp::DirectionalLight> dirLight = nullptr;
	Ref<Capp::Spotlight> spotlight       = nullptr;

	Ref<Capp::TextureCubemap> skybox = nullptr;
	
	Ref<Capp::Texture3D> coolLUT   = nullptr;
	Ref<Capp::Texture3D> warmLUT   = nullptr;
	Ref<Capp::Texture3D> customLUT = nullptr;
	
	Capp::PostProcessingPass grayscale     = { nullptr, nullptr };
	Capp::PostProcessingPass inversion     = { nullptr, nullptr };
	Capp::PostProcessingPass colourGrading = { nullptr, nullptr };

	bool isGammaCorrected = false;
	float gamma = 2.2f;
	
	bool isGray = false;
	bool isInverted = false;
	
	int lutSetting = 0;
};
