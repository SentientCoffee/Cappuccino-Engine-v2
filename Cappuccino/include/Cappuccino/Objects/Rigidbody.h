#pragma once

#include "Cappuccino/Objects/Hitbox.h"

namespace Capp {

	struct Physics {

		static const float gravity;
		static const float universalG;
		
	};

	class RigidBody {

		using HitboxVector        = std::vector<Ref<Hitbox>>;
		using HitboxInitList      = std::initializer_list<Ref<Hitbox>>;
		using HitboxIterator      = std::vector<Ref<Hitbox>>::iterator;
		using ConstHitboxIterator = std::vector<Ref<Hitbox>>::const_iterator;
		
	public:

		RigidBody() = default;
		RigidBody(const HitboxVector& hitboxes);
		RigidBody(const HitboxInitList& hitboxes);
		~RigidBody();

		RigidBody& operator=(const HitboxVector& hitboxes);
		RigidBody& operator=(const HitboxInitList& hitboxes);
		
		bool isMovable() const;
		void setMovable(bool movable);

		bool isTangible() const;
		void setTangible(bool tangible);

		bool hasGravity() const;
		void setGravity(bool gravity);

		float getMass() const;
		void setMass(float mass);

		void setAcceleration(const glm::vec3& acceleration);
		void addForce(const glm::vec3& force);

		void setVelocity(const glm::vec3& velocity);
		void addAcceleration(const glm::vec3& acceleration);

		void addVelocity(const glm::vec3& velocity);
		RigidBody& setPosition(const glm::vec3& position);
		RigidBody& setPosition(float x, float y, float z);
		const glm::vec3& getPosition() const;

		RigidBody& setRotation(const glm::vec3& degrees);
		RigidBody& setRotation(float x, float y, float z);
		const glm::vec3& getRotation() const;

		RigidBody& setScale(const glm::vec3& scale);
		RigidBody& setScale(float x, float y, float z);
		RigidBody& setScale(float scale);
		const glm::vec3& getScale() const;

		Transform& getTransform();

		bool checkCollision(const RigidBody& other);
		bool checkCollision(const Ref<Hitbox>& other);

		HitboxVector getHitboxes() const;

		HitboxIterator begin();
		HitboxIterator end();
		ConstHitboxIterator begin() const;
		ConstHitboxIterator end() const;
		
	private:

		bool _isMovable = true;
		bool _isTangible = true;
		
		bool _hasGravity = false;
		float _mass = 1.0f;

		glm::vec3 _acceleration = { 0.0f, 0.0f, 0.0f };
		glm::vec3 _velocity = { 0.0f, 0.0f, 0.0f };

		const glm::vec3 _maxAccel = { 20.0f, 20.0f, 20.0f };
		const glm::vec3 _maxVelocity = { 20.0f, 20.0f, 20.0f };
		
		Transform _transform;
		
		HitboxVector _hitboxes;
		
	};
	
}
