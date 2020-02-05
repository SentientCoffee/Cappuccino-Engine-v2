#pragma once

#include "Cappuccino/Rendering/VertexArray.h"

#include <string>
#include <tuple>
#include <vector>

namespace Capp {

	class Mesh {
	public:

		Mesh(const std::string& name, const std::string& filepath);
		Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices);
		~Mesh();
		
		VertexArray* getVAO() const;
		
	private:
		
		VertexArray* _vao;
		std::string _name;
		std::string _meshPath;

	};
}