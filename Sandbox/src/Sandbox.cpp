#include <Cappuccino.h>
#include <Cappuccino/EntryPoint.h>

#include "Scenes/Scene1.h"

#define SCR_WIDTH    1280
#define SCR_HEIGHT   720
#define SCR_TITLE    "Cappuccino Engine v2 Test"

class Sandbox final : public Capp::Application {
public:
	Sandbox() : Application(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE) {
		getWindow()->setVSyncEnabled(true);
		
		const auto scene = new TestScene;
		
		Capp::SceneManager::addScene(scene);
		Capp::SceneManager::changeScene(scene);
	}
	
	~Sandbox() = default;
	
};

Capp::Application* Capp::createApp() { return new Sandbox; }