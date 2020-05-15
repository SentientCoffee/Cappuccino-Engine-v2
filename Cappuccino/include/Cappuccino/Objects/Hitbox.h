#pragma once

#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Objects/Transform.h"
#include "Cappuccino/Rendering/VertexArray.h"

#include <glm/glm.hpp>
#include <tuple>
#include <vector>

namespace Capp {

	enum class HitboxType : unsigned int {
		None = 0,
		Cube,
		Sphere
	};
	
	class Hitbox {
	public:
		
		virtual ~Hitbox() = default;

		virtual HitboxType getHitboxType() const = 0;

		Ref<VertexArray> getVAO() const { return _vao; }

		Hitbox& setPosition(const glm::vec3& position);
		Hitbox& setRotation(const glm::vec3& degrees);

		const glm::vec3& getPosition() const { return _transform.getPosition(); }
		Hitbox& setPosition(const float x, const float y, const float z) { return setPosition({ x, y, z }); }

		const glm::vec3& getRotation() const { return _transform.getRotation(); }
		Hitbox& setRotation(const float x, const float y, const float z) { return setRotation({ x, y, z }); }

		Transform& getTransform() { return _transform; }
		
		static void setAllVisible(const bool draw) { _drawHitboxes = draw; }
		static bool isVisible() { return _drawHitboxes; }

	protected:

		Transform _transform;
		Ref<VertexArray> _vao = nullptr;

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

		HitboxType getHitboxType() const override { return HitboxType::Cube; }
		
		const glm::vec3& getScale() const;
		Transform& setScale(const glm::vec3& scale);
		Transform& setScale(float x, float y, float z);
		Transform& setScale(float scale);
		
		bool checkCollision(const Ref<HitboxCube>& other) const;
		bool checkCollision(const Ref<HitboxSphere>& other) const;

	private:

		std::tuple<float, float> getSATMinMax(const glm::vec3& axis) const;
		static float getDistance(float spherePos, float boxPos, float boxSize);
		
		std::vector<Vertex> _vertices;
	};

	class HitboxSphere : public Hitbox {

		friend HitboxCube;
		
	public:

		HitboxSphere() = default;
		HitboxSphere(const glm::vec3& position, float radius);
		~HitboxSphere() = default;

		HitboxType getHitboxType() const override { return HitboxType::Sphere; }
		
		float getScale() const;
		Transform& setScale(float scale);

		bool checkCollision(const Ref<HitboxSphere>& other) const;
		bool checkCollision(const Ref<HitboxCube>& other) const;
		
	};
	
}
