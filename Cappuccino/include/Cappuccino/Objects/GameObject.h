#pragma once

#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Events/Event.h"
#include "Cappuccino/Objects/Rigidbody.h"
#include "Cappuccino/Rendering/3D/Model.h"

#include <string>

namespace Capp {

	class GameObject {

		using ModelList = std::vector<Ref<Model>>;
		
	public:

		GameObject();
		virtual ~GameObject() = default;

		template<class Object, typename = typename std::enable_if<std::is_base_of<GameObject, Object>::value>::type>
		static Ref<Object> create() {
			return Memory::createRef<Object>();
		}

		bool operator==(const GameObject& other) const { return _objectId == other._objectId; }

		virtual void update() = 0;
		virtual void onEvent(Event& e) = 0;

		unsigned getId() const { return _objectId; }

		const std::string& getName() const { return _name; }
		void setName(const std::string& name) { _name = name; }
		
		const std::string& getTag() const { return _tag; }
		void setTag(const std::string& tag) { _tag = tag; }

		bool isActive() const { return _isActive; }
		void setActive(const bool active) { _isActive = active; }

		bool isVisible() const { return _isVisible; }
		void setVisible(const bool visible) { _isVisible = visible; }

		const glm::vec3& getPosition() const;
		GameObject& setPosition(const glm::vec3& position);
		GameObject& setPosition(float x, float y, float z);
		GameObject& translateBy(const glm::vec3& translation);
		GameObject& translateBy(float x, float y, float z);
		
		const glm::vec3& getRotation() const;
		GameObject& setRotation(const glm::vec3& degrees);
		GameObject& setRotation(float x, float y, float z);
		GameObject& rotateBy(const glm::vec3& degrees);
		GameObject& rotateBy(float x, float y, float z);
		
		const glm::vec3& getScale() const;
		GameObject& setScale(const glm::vec3& scale);
		GameObject& setScale(float x, float y, float z);
		GameObject& setScale(float scale);
		GameObject& scaleBy(const glm::vec3& scale);
		GameObject& scaleBy(float x, float y, float z);
		GameObject& scaleBy(float scale);

		bool checkCollision(Ref<GameObject> other);
		
		const RigidBody& getRigidBody() const { return _rigidbody; }
		ModelList getModels() const { return _models; }
		
	protected:

		unsigned _objectId = 0;
		std::string _name;
		std::string _tag;

		bool _isActive = true;
		bool _isVisible = true;

		RigidBody _rigidbody;
		ModelList _models;

	private:
		
		static unsigned _objectCount;
		
	};
	
}
