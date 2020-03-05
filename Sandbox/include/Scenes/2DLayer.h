#pragma once

#include <Cappuccino.h>

class Layer2D : public Capp::Layer {
public:
	Layer2D() : Layer("2D Layer") {}
	~Layer2D() = default;


	void onPush() override;
	void onPop() override;

	void update() override;

	void onEvent(Capp::Event& e) override;

	void drawImgui() override;

private:

	Capp::Text* controls = nullptr;
	Capp::Text* escape = nullptr;
	Capp::Text* flashlight = nullptr;

};