#pragma once

#include "Cappuccino/Scenes/Scene.h"

namespace Capp {

	class SceneManager {

		using SceneVector = std::vector<Scene*>;

	public:

		static void init();
		static void shutdown();
		
		static void addScene(Scene* scene);
		static void removeScene(Scene* scene);
		
		static void changeScene(Scene* scene);
		static void changeScene(const std::string& name);
		static void changeScene(int index);

		static Scene* getCurrentScene();

		static Scene* getScene(const std::string& name);
		static Scene* getScene(int index);

	private:

		static SceneVector _scenes;
		static Scene* _currentScene;

	};
	
}