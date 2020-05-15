#pragma once

#include "Cappuccino/Rendering/Mesh.h"
#include "Cappuccino/Rendering/3D/Material.h"
#include "Cappuccino/Objects/Transform.h"

namespace Capp {

	class Model {
	public:

		Model() = default;
		Model(const Ref<Mesh>& mesh, const Ref<Material>& material);
		~Model() = default;

		static Ref<Model> create(const Ref<Mesh>& mesh, const Ref<Material>& material) {
			return Memory::createRef<Model>(mesh, material);
		}

		Ref<Mesh> getMesh() const { return _mesh; }
		void setMesh(const Ref<Mesh>& mesh) { _mesh = mesh; }

		Ref<Material> getMaterial() const { return _material; }
		void setMaterial(const Ref<Material>& material) { _material = material; }

		const glm::vec3& getPosition() const;
		Model& setPosition(const glm::vec3& position);
		Model& setPosition(float x, float y, float z);

		const glm::vec3& getRotation() const;
		Model& setRotation(const glm::vec3& degrees);
		Model& setRotation(float x, float y, float z);

		const glm::vec3& getScale() const;
		Model& setScale(const glm::vec3& scale);
		Model& setScale(float x, float y, float z);
		Model& setScale(float scale);

		const Transform& getTransform() const { return _transform; }
		
	private:

		Ref<Mesh> _mesh;
		Ref<Material> _material;
		
		Transform _transform;
		
	};
	
}
