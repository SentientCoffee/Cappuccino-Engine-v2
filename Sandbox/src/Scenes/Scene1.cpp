#include "Scenes/Scene1.h"

#include "Scenes/2DLayer.h"
#include "Scenes/3DLayer.h"

void TestScene::init() {
	pushLayer(new Layer3D);
	//pushLayer(new Layer2D);
}
void TestScene::exit() {}

void TestScene::drawImgui() {
	ImGui::Begin(getName().c_str());
	ImGui::Text("Time running: %f", Capp::Time::getTime());
	ImGui::Text("Frame time: %f", Capp::Time::getDeltaTime());
	ImGui::End();
}
