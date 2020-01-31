#pragma once

#include "Cappuccino/Events/Event.h"
#include "Cappuccino/Objects/Rigidbody.h"
#include "Cappuccino/Rendering/3D/Model.h"

#include <string>

namespace Capp {

	class GameObject {

		using ModelVector = std::vector<Model*>;
		using ModelInitList = std::initializer_list<Model*>;
		
	public:

		GameObject();
		GameObject(const ModelVector& models);
		GameObject(const ModelInitList& models);
		virtual ~GameObject();

		GameObject* operator=(const ModelVector& models);
		GameObject* operator=(const ModelInitList& models);

		bool operator==(const GameObject& other) const;

		virtual void init();
		virtual void exit();
		virtual void update();
		virtual void onEvent(Event& e);

		unsigned getId() const;

		const std::string& getName() const;
		void setName(const std::string& name);
		
		const std::string& getTag() const;
		void setTag(const std::string& tag);

		bool isActive() const;
		void setActive(bool active);

		bool isVisible() const;
		void setVisible(bool visible);

		const glm::vec3& getPosition() const;
		GameObject& setPosition(const glm::vec3& position);
		GameObject& setPosition(float x, float y, float z);
		
		const glm::vec3& getRotation() const;
		GameObject& setRotation(const glm::vec3& eulerRotation);
		GameObject& setRotation(float x, float y, float z);

		
		const glm::vec3& getScale() const;
		GameObject& setScale(const glm::vec3& scale);
		GameObject& setScale(float x, float y, float z);
		GameObject& setScale(float scale);

		bool checkCollision(GameObject* other);
		
		const RigidBody& getRigidBody() const;

		ModelVector getModels() const;
		
		static std::vector<GameObject*> gameObjects;
		
	protected:

		unsigned _objectId = 0;
		std::string _name;
		std::string _tag;

		bool _isActive = true;
		bool _isVisible = true;

		RigidBody _rigidbody;
		ModelVector _models;

	private:
		
		static unsigned objectCount;
		
	};
	
}
