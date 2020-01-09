#include "CappPCH.h"
#include "Cappuccino/Scenes/SceneManager.h"

using namespace Capp;

SceneManager::SceneManager() :
	_currentScene(nullptr) {}

SceneManager::~SceneManager() {
	for(auto scene : _scenes) {
		delete scene;
	}

	_scenes.clear();
}


void SceneManager::addScene(Scene* scene) {
	_scenes.push_back(scene);
}

void SceneManager::removeScene(Scene* scene) {
	const auto it = std::find(_scenes.begin(), _scenes.end(), scene);

	if(it != _scenes.end()) {
		_scenes.erase(it);
	}
}

void SceneManager::changeScene(Scene* scene) {
	if(scene == _currentScene || scene == nullptr) {
		return;
	}

	const auto it = std::find(_scenes.begin(), _scenes.end(), scene);
	if(it == _scenes.end()) {
		return;
	}

	if(_currentScene != nullptr) {
		_currentScene->exit();
	}

	_currentScene = *it;
	_currentScene->init();
}

void SceneManager::changeScene(const std::string& name) {
	Scene* scene = getScene(name);
	changeScene(scene);
}

void SceneManager::changeScene(const int index) {
	Scene* scene = getScene(index);
	changeScene(scene);
}

Scene* SceneManager::getCurrentScene() const { return _currentScene; }

Scene* SceneManager::getScene(const std::string& name) const {
	for(auto scene : _scenes) {
		if(scene->_sceneName == name) {
			return scene;
		}
	}

	return nullptr;
}

Scene* SceneManager::getScene(const int index) const {
	if(index < 0 || index >= static_cast<int>(_scenes.size())) {
		return nullptr;
	}

	return _scenes[index];
}

SceneManager::SceneIterator SceneManager::begin() { return _scenes.begin(); }
SceneManager::ConstSceneIterator SceneManager::begin() const { return _scenes.begin(); }
SceneManager::SceneIterator SceneManager::end() { return _scenes.end(); }
SceneManager::ConstSceneIterator SceneManager::end() const { return _scenes.end(); }