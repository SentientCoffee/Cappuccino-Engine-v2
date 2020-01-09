#pragma once

#include "Cappuccino/Scenes/Scene.h"

namespace Capp {

	class SceneManager {

		using SceneVector = std::vector<Scene*>;
		using SceneIterator = std::vector<Scene*>::iterator;
		using ConstSceneIterator = std::vector<Scene*>::const_iterator;

	public:

		SceneManager();
		~SceneManager();

		void addScene(Scene* scene);
		void removeScene(Scene* scene);

		void changeScene(Scene* scene);
		void changeScene(const std::string& name);
		void changeScene(int index);

		Scene* getCurrentScene() const;

		Scene* getScene(const std::string& name) const;
		Scene* getScene(int index) const;

		SceneIterator begin();
		ConstSceneIterator begin() const;
		SceneIterator end();
		ConstSceneIterator end() const;

	private:

		SceneVector _scenes;
		Scene* _currentScene;

	};
	
}