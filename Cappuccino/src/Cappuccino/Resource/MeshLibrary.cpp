#include "CappPCH.h"
#include "Cappuccino/Resource/MeshLibrary.h"

using namespace Capp;

MeshLibrary::MeshMap MeshLibrary::_meshes;
bool MeshLibrary::_initialized = false;

void MeshLibrary::init() {
	if(_initialized) {
		CAPP_ASSERT(!_initialized, "Mesh library already initialized!");
		return;
	}
	
	_initialized = true;
}

void MeshLibrary::shutdown() {
	_meshes.clear();
}

void MeshLibrary::addMesh(const std::string& name, const Ref<Mesh>& mesh) {
	if(hasMesh(name)) {
		CAPP_ASSERT(!hasMesh(name), "Mesh \"{0}\" already exists!", name);
		return;
	}

	_meshes[name] = mesh;
}

Ref<Mesh> MeshLibrary::loadMesh(const std::string& name, const std::string& filepath) {
	if(hasMesh(name)) {
		CAPP_PRINT_INFO("Mesh \"{0}\" already exists, using loaded mesh...", name);
		return _meshes[name];
	}

	return _meshes[name] = Mesh::create(name, filepath);
}

Ref<Mesh> MeshLibrary::getMesh(const std::string& name) {
	if(!hasMesh(name)) {
		CAPP_ASSERT(hasMesh(name), "Mesh \"{0}\" not found!", name);
		return nullptr;
	}

	return _meshes[name];
}

bool MeshLibrary::hasMesh(const std::string& name) { return _meshes.find(name) != _meshes.end(); }