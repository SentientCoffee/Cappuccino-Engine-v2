#include "CappPCH.h"
#include "Cappuccino/Scenes/Scene.h"

#include "Cappuccino/Objects/GameObject.h"

using namespace Capp;

Scene::Scene(const std::string& name) :
	_sceneName(name) {
	_imguiLayer = new ImguiLayer;
	pushOverlay(_imguiLayer);
}

void Scene::init() {}
void Scene::exit() {}
void Scene::drawImgui() {}

void Scene::update(const float dt) {
	for(auto layer : _layerStack) {
		layer->update(dt);
	}

	for(auto gameObject : GameObject::gameObjects) {
		if(gameObject->isActive()) {
			gameObject->update(dt);
		}
	}
	
	#if CAPP_DEBUG
	
	_imguiLayer->begin();
	drawImgui();
	for(auto layer : _layerStack) {
		layer->updateImgui(dt);
	}
	_imguiLayer->end();
	
	#endif
}

void Scene::onEvent(Event& e) {
	for(auto it = _layerStack.end(); it != _layerStack.begin(); ) {
		if(e.isHandled()) { break; }
		(*--it)->onEvent(e);
	}

	for(auto gameObject : GameObject::gameObjects) {
		if(gameObject->isActive()) {
			if(e.isHandled()) { break; }
			gameObject->onEvent(e);
		}
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
