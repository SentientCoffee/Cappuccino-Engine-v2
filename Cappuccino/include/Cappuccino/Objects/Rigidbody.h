#pragma once

#include "Cappuccino/Objects/Hitbox.h"

namespace Capp {

	class Physics {
	public:

		static const float gravity;
		static const float universalG;
		
	};

	class RigidBody {

		using HitboxVector = std::vector<Hitbox*>;
		using HitboxInitList = std::initializer_list<Hitbox*>;
		using HitboxVectorIterator = std::vector<Hitbox*>::iterator;
		using ConstHitboxVectorIterator = std::vector<Hitbox*>::const_iterator;
		
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
		const glm::vec3& getPosition() const;

		RigidBody& setRotation(const glm::vec3& eulerRotation);
		const glm::vec3& getRotation() const;

		RigidBody& setScale(float scale);
		float getScale() const;

		bool checkCollision(const RigidBody& other);
		bool checkCollision(Hitbox* other);

		HitboxVector getHitboxes() const;

		HitboxVectorIterator begin();
		HitboxVectorIterator end();
		ConstHitboxVectorIterator begin() const;
		ConstHitboxVectorIterator end() const;
		
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
		//std::vector<glm::vec3> _posOffsets;
		//std::vector<glm::vec3> _rotOffsets;
		//std::vector<glm::vec3> _originalScales;
		
	};
	
}
