#include "CappPCH.h"
#include "Cappuccino/Scenes/Scene.h"

using namespace Capp;

Scene::Scene(const std::string& name) :
	_sceneName(name) {
	_imguiLayer = new ImguiLayer;
	pushOverlay(_imguiLayer);
}

Scene::~Scene() {
	for(auto gameObject : _gameObjects) {
		delete gameObject;
	}

	_gameObjects.clear();
}

void Scene::init() {}
void Scene::exit() {}
void Scene::drawImgui() {}

void Scene::update() {

	for(const auto& gameObject : _gameObjects) {
		if(gameObject->isActive()) {
			gameObject->update();
		}
	}

	for(auto layer : _layerStack) {
		layer->update();
	}
	
	#if CAPP_DEBUG || CAPP_RELEASE
	
	ImguiLayer::begin();
	drawImgui();
	for(auto layer : _layerStack) {
		layer->drawImgui();
	}
	ImguiLayer::end();
	
	#endif
}

void Scene::onEvent(Event& e) {
	for(auto it = _layerStack.end(); it != _layerStack.begin(); ) {
		if(e.isHandled()) { break; }
		(*--it)->onEvent(e);
	}

	for(const auto& gameObject : _gameObjects) {
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

void Scene::addGameObject(GameObject* gameObject) {
	_gameObjects.push_back(gameObject);
}

const std::string& Scene::getName() const { return _sceneName; }
