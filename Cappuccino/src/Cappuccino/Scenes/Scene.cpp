#include "CappPCH.h"
#include "Cappuccino/Scenes/Scene.h"

using namespace Capp;

Scene::Scene(const std::string& name) :
	_sceneName(name) {
	_imguiLayer = new ImguiLayer;
	pushOverlay(_imguiLayer);
}

void Scene::init() {}
void Scene::exit() {}

void Scene::update(const float dt) {
	for(auto layer : _layerStack) {
		layer->update(dt);
	}

	_imguiLayer->begin();
	for(auto layer : _layerStack) {
		layer->updateImgui(dt);
	}
	_imguiLayer->end();

}

void Scene::onEvent(Event& e) {
	for(auto it = _layerStack.end(); it != _layerStack.begin(); ) {
		(*--it)->onEvent(e);
		if(e.isHandled()) { break; }
	}
}

void Scene::pushLayer(Layer* layer) {
	_layerStack.pushLayer(layer);
	layer->onPush();
}
void Scene::pushOverlay(Layer* overlay) {
	_layerStack.pushOverlay(overlay);
	overlay->onPush();
}

const std::string& Scene::getName() const { return _sceneName; }
