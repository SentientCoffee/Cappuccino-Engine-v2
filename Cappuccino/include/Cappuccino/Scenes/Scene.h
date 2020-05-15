#pragma once

#include "Cappuccino/Events/Event.h"
#include "Cappuccino/Scenes/LayerStack.h"
#include "Cappuccino/Debugging/ImguiLayer.h"
#include "Cappuccino/Objects/GameObject.h"

namespace Capp {

	class SceneManager;

	class Scene {

		friend SceneManager;
		
		using GameObjectList = std::vector<GameObject*>;

	public:

		Scene(const std::string& name);
		virtual ~Scene();

		virtual void init();
		virtual void exit();
		virtual void drawImgui();
		void update();
		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		void addGameObject(GameObject* gameObject);

		const std::string& getName() const;

	protected:

		LayerStack _layerStack;
		GameObjectList _gameObjects;
		ImguiLayer* _imguiLayer;
		
		std::string _sceneName;
	};
	
}
