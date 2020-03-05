#pragma once
#include <Cappuccino.h>

#include "Objects/CameraController.h"
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
	
	SentryBot* sentry1 = nullptr;
	SentryBot* sentry2 = nullptr;
	F16* f16 = nullptr;
	
	Capp::PointLight* pointLight = nullptr;
	Capp::DirectionalLight* dirLight = nullptr;
	Capp::Spotlight* spotlight = nullptr;

	Capp::TextureCubemap* skybox = nullptr;
	
	Capp::Texture3D* coolLUT = nullptr;
	Capp::Texture3D* warmLUT = nullptr;
	Capp::Texture3D* customLUT = nullptr;
	
	Capp::PostProcessingPass grayscale = { nullptr, nullptr };
	Capp::PostProcessingPass inversion = { nullptr, nullptr };
	Capp::PostProcessingPass colourGrading = { nullptr, nullptr };

	bool isGray = false;
	bool isInverted = false;
	int lutSetting = 0;
};