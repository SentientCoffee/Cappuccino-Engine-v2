#pragma once

#include <glm/glm.hpp>

namespace Capp {

	class Transform {
	public:

		Transform() = default;
		~Transform() = default;

		const glm::mat4& getLocalTransform() const;
		const glm::mat4& getWorldTransform() const;

		const glm::vec3& getPosition() const;
		Transform& setPosition(const glm::vec3& position);
		Transform& setPosition(float x, float y, float z);

		const glm::vec3& getRotation() const;
		Transform& setRotation(const glm::vec3& eulerRotation);
		Transform& setRotation(float x, float y, float z);

		const glm::vec3& getScale() const;
		Transform& setScale(const glm::vec3& scale);
		Transform& setScale(float x, float y, float z);
		Transform& setScale(float scale);

		const glm::mat4& getParentTransform() const;
		void setParentTransform(const glm::mat4& parent);
		
	private:

		void calculateTransform() const;
		
		mutable bool _transformChanged = false;
		mutable glm::mat4 _localTransform = glm::mat4(1.0f);
		mutable glm::mat4 _worldTransform = glm::mat4(1.0f);
		
		glm::vec3 _position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 _rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 _scale    = { 1.0f, 1.0f, 1.0f };

		glm::mat4 _parentTransform = glm::mat4(1.0f);
	};
	
}