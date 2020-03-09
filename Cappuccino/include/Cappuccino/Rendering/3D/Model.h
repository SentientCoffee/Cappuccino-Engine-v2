#pragma once

#include "Cappuccino/Rendering/Mesh.h"
#include "Cappuccino/Rendering/3D/Material.h"
#include "Cappuccino/Objects/Transform.h"

namespace Capp {

	class Model {
	public:

		Model(Mesh* mesh, Material* material);
		~Model();

		Mesh* getMesh() const;
		void setMesh(Mesh* mesh);

		Material* getMaterial() const;
		void setMaterial(Material* material);

		const glm::vec3& getPosition() const;
		Model& setPosition(const glm::vec3& position);
		Model& setPosition(float x, float y, float z);

		const glm::vec3& getRotation() const;
		Model& setRotation(const glm::vec3& eulerRotation);
		Model& setRotation(float x, float y, float z);

		const glm::vec3& getScale() const;
		Model& setScale(const glm::vec3& scale);
		Model& setScale(float x, float y, float z);
		Model& setScale(float scale);

		const Transform& getTransform() const;
		
	private:

		Mesh* _mesh;
		Material* _material;
		
		Transform _transform;
		
	};
	
}
