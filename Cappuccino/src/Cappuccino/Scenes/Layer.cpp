#include "CappPCH.h"
#include "Cappuccino/Scenes/Layer.h"

#include <imgui/imgui.h>

using namespace Capp;

Layer::Layer(const std::string& name) :
	_layerName(name) {}

void Layer::onPush() {}
void Layer::onPop() {}
void Layer::update(const float dt) {}
void Layer::drawImgui() {}
void Layer::onEvent(Event& e) {}

void Layer::updateImgui(const float dt) {
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = dt;
	drawImgui();
}

const std::string& Layer::getName() const { return _layerName; }
