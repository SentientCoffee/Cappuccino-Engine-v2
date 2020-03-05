#pragma once

#include <Cappuccino.h>

class TestScene : public Capp::Scene {
public:

	TestScene() : Scene("Scene 1") {}
	~TestScene() = default;

	void init() override;
	void exit() override;

	void drawImgui() override;
	
};
