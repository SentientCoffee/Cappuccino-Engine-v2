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

	Capp::Quad bg;
	Ref<Capp::Text> controls = nullptr;
	Ref<Capp::Text> escape = nullptr;
	Ref<Capp::Text> flashlight = nullptr;

};