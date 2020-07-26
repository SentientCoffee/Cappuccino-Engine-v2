#pragma once

#include "3D/Material.h"
#include "Cappuccino/Rendering/VertexArray.h"

#include <string>
#include <vector>

namespace Capp {

	class Mesh {
	public:

		Mesh() = default;
		Mesh(const std::string& name, const std::string& filepath);
		Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
		Mesh(const std::string& name, const Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount);

		template<size_t VSize, size_t ISize>
		Mesh(const std::string& name, const std::array<Vertex, VSize>& vertices, const std::array<uint32_t, ISize>& indices) :
			Mesh(name, vertices.data(), VSize, indices.data(), ISize) {}
		
		~Mesh() = default;

		static Ref<Mesh> create(const std::string& name, const std::string& filepath) {
			return Memory::createRef<Mesh>(name, filepath);
		}
		static Ref<Mesh> create(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
			return Memory::createRef<Mesh>(name, vertices, indices);
		}
		static Ref<Mesh> create(const std::string& name, Vertex* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount) {
			return Memory::createRef<Mesh>(name, vertices, vertexCount, indices, indexCount);
		}
		template<size_t VSize, size_t ISize>
		static Ref<Mesh> create(const std::string& name, const std::array<Vertex, VSize>& vertices, const std::array<uint32_t, ISize>& indices) {
			return Memory::createRef<Mesh>(name, vertices, indices);
		}
		
		Ref<VertexArray> getVAO() const { return _vao; }

		void setMaterial(const Ref<Material>& material) { _material = material; }
		Ref<Material> getMaterial() const { return _material; }
		
	private:

		Ref<Material> _material;
		Ref<VertexArray> _vao;
		std::string _name;

	};
}
