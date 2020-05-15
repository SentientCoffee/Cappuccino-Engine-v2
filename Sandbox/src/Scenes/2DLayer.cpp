#include "Scenes/2DLayer.h"

void Layer2D::onPush() {

	const auto robotoMedium = Capp::FontLibrary::loadFont("Roboto Medium", "Assets/Cappuccino/Fonts/Roboto-Medium.ttf");

	bg = {
		{ 210.0f, 670.0f, -1.0f },
		{ 400.0f, 70.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};
	
	controls = Capp::Text::create("W A S D Z X to move camera", robotoMedium);
	controls->setPosition(20.0f, 655.0f).setScale(0.4f);

	escape = Capp::Text::create("Escape to toggle the mouse cursor's visibility", robotoMedium);
	escape->setPosition(20.0f, 675.0f).setScale(0.4f);

	flashlight = Capp::Text::create("F to toggle flashlight", robotoMedium);
	flashlight->setPosition(20.0f, 695.0f).setScale(0.4f);
}

void Layer2D::onPop() {}

void Layer2D::update() {

	Capp::Renderer2D::start();
	{
		Capp::Renderer2D::drawQuad(bg);
		Capp::Renderer2D::drawText(controls);
		Capp::Renderer2D::drawText(escape);
		Capp::Renderer2D::drawText(flashlight);
	}
	Capp::Renderer2D::finish();
}

void Layer2D::onEvent(Capp::Event& e) {}

void Layer2D::drawImgui() {
	
}