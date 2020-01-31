#include "CappPCH.h"
#include "Cappuccino/Scenes/Layer.h"

#include <imgui/imgui.h>

using namespace Capp;

Layer::Layer(const std::string& name) :
	_layerName(name) {}

void Layer::onPush() {}
void Layer::onPop() {}
void Layer::update() {}
void Layer::drawImgui() {}
void Layer::onEvent(Event& e) {}

const std::string& Layer::getName() const { return _layerName; }
