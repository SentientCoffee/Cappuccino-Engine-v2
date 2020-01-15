#pragma once

#include "Cappuccino/Rendering/3D/Mesh.h"

#include <string>
#include <unordered_map>

namespace Capp {

	class MeshLibrary {
	public:

		static void init();
		static void shutdown();

		static void addMesh(const std::string& name, Mesh* mesh);
		static Mesh* loadMesh(const std::string& name, const std::string& filepath);
		static Mesh* getMesh(const std::string& name);

	private:

		static bool hasMesh(const std::string& name);
		
		static std::unordered_map<std::string, Mesh*> _meshes;
		static bool _initialized;
		
	};
	
}