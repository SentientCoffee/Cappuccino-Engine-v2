#pragma once

#include "Cappuccino/Objects/Transform.h"
#include "Cappuccino/Rendering/VertexArray.h"

#include <glm/glm.hpp>

#include <tuple>
#include <vector>

namespace Capp {

	class Hitbox {
	public:
		
		virtual ~Hitbox();

		VertexArray* getVAO() const;

		const glm::vec3& getPosition() const;
		Transform& setPosition(const glm::vec3& position);
		Transform& setPosition(float x, float y, float z);

		const glm::vec3& getRotation() const;
		Transform& setRotation(const glm::vec3& eulerRotation);
		Transform& setRotation(float x, float y, float z);

		const Transform& getTransform() const;
		
		static void setShouldDraw(bool draw);
		static bool shouldDraw();

	protected:

		Transform _transform;
		VertexArray* _vao = nullptr;

	private:
		
		static bool _drawHitboxes;
		
	};

	class HitboxSphere;
	class HitboxCube : public Hitbox {

		friend HitboxSphere;
		
	public:

		HitboxCube() = default;
		HitboxCube(const glm::vec3& position, const glm::vec3& dimensions, const glm::vec3& rotation = glm::vec3(0.0f));
		~HitboxCube() = default;

		const glm::vec3& getScale() const;
		Transform& setScale(const glm::vec3& scale);
		Transform& setScale(float x, float y, float z);
		Transform& setScale(float scale);
		
		bool checkCollision(HitboxCube* other) const;
		bool checkCollision(HitboxSphere* other) const;

	private:

		std::tuple<float, float> getSATMinMax(const glm::vec3& axis) const;
		float getDistance(float spherePos, float boxPos, float boxSize) const;
		
		std::vector<Vertex> _vertices;
	};

	class HitboxSphere : public Hitbox {

		friend HitboxCube;
		
	public:

		HitboxSphere() = default;
		HitboxSphere(const glm::vec3& position, float radius);
		~HitboxSphere() = default;

		float getScale() const;
		Transform& setScale(float scale);

		bool checkCollision(HitboxSphere* other) const;
		bool checkCollision(HitboxCube* other);
		
	};
	
}
