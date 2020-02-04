#include "CappPCH.h"
#include "Cappuccino/Scenes/SceneManager.h"

using namespace Capp;

Scene* SceneManager::_currentScene;
std::vector<Scene*> SceneManager::_scenes;

void SceneManager::init() {
	_currentScene = nullptr;
	_scenes.reserve(20);
}

void SceneManager::shutdown() {
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

Scene* SceneManager::getCurrentScene() { return _currentScene; }

Scene* SceneManager::getScene(const std::string& name) {
	for(auto scene : _scenes) {
		if(scene->_sceneName == name) {
			return scene;
		}
	}

	return nullptr;
}

Scene* SceneManager::getScene(const int index) {
	if(index < 0 || index >= static_cast<int>(_scenes.size())) {
		return nullptr;
	}

	return _scenes[index];
}
