#pragma once

#include "Cappuccino/Events/Event.h"
#include "Cappuccino/Scenes/LayerStack.h"
#include "Cappuccino/Debugging/ImguiLayer.h"

namespace Capp {

	class SceneManager;

	class Scene {

		friend SceneManager;

	public:

		Scene(const std::string& name = "Scene");
		virtual ~Scene() = default;

		virtual void init();
		virtual void exit();
		void update(float dt);
		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		const std::string& getName() const;

	protected:

		LayerStack _layerStack;
		ImguiLayer* _imguiLayer;
		
		std::string _sceneName;
	};
	
}
