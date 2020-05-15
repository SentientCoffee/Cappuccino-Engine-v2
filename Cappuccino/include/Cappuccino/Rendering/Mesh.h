#pragma once

#include "Cappuccino/Rendering/VertexArray.h"

#include <string>
#include <vector>

namespace Capp {

	class Mesh {
	public:

		Mesh() = default;
		Mesh(const std::string& name, const std::string& filepath);
		Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices);
		~Mesh() = default;

		static Ref<Mesh> create(const std::string& name, const std::string& filepath) {
			return Memory::createRef<Mesh>(name, filepath);
		}
		
		static Ref<Mesh> create(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices) {
			return Memory::createRef<Mesh>(name, vertices, indices);
		}
		
		Ref<VertexArray> getVAO() const { return _vao; }
		
	private:
		
		Ref<VertexArray> _vao;
		std::string _name;

	};
}