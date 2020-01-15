#pragma once

#include "Cappuccino/Rendering/3D/Mesh.h"
#include "Cappuccino/Rendering/3D/Material.h"
#include "Cappuccino/Objects/Transform.h"

namespace Capp {

	class Model {
	public:

		Model() = default;
		Model(Mesh* mesh, Material* material);
		~Model();

		Mesh* getMesh() const;
		void setMesh(Mesh* mesh);

		Material* getMaterial() const;
		void setMaterial(Material* material);

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

		const Transform& getTransform() const;
		
	private:

		Mesh* _mesh = nullptr;
		Material* _material = nullptr;
		
		Transform _transform;
		
	};
	
}
