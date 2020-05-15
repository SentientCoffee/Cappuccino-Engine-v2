#pragma once

#include "Cappuccino/Rendering/Mesh.h"

#include <string>
#include <unordered_map>

namespace Capp {

	class MeshLibrary {

		using MeshMap = std::unordered_map<std::string, Ref<Mesh>>;
		
	public:

		static void init();
		static void shutdown();

		static void addMesh(const std::string& name, const Ref<Mesh>& mesh);
		static Ref<Mesh> loadMesh(const std::string& name, const std::string& filepath);
		static Ref<Mesh> getMesh(const std::string& name);

	private:

		static bool hasMesh(const std::string& name);
		
		static MeshMap _meshes;
		static bool _initialized;
		
	};
	
}