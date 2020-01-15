#pragma once

#include "Cappuccino/Events/Event.h"
#include "Cappuccino/Objects/Transform.h"
#include "Cappuccino/Rendering/3D/Model.h"

#include <string>

namespace Capp {

	class GameObject {
	public:

		GameObject();
		virtual ~GameObject() = default;

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

		const glm::vec3& getPosition() const;
		void setPosition(const glm::vec3& position);
		void setPosition(float x, float y, float z);
		
		const glm::vec3& getRotation() const;
		void setRotation(const glm::vec3& eulerRotation);
		void setRotation(float x, float y, float z);

		
		const glm::vec3& getScale() const;
		void setScale(const glm::vec3& scale);
		void setScale(float x, float y, float z);
		void setScale(float scale);
		
		
	protected:

		unsigned _objectId;
		std::string _name;
		std::string _tag;

		Transform _transform;
		std::vector<Model*> _models;

	private:
		
		static unsigned objectCount;
	};
	
}
