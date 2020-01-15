#include "CappPCH.h"
#include "Cappuccino/Resource/MeshLibrary.h"

using namespace Capp;

std::unordered_map<std::string, Mesh*> MeshLibrary::_meshes;
bool MeshLibrary::_initialized = false;

void MeshLibrary::init() {
	if(_initialized) {
		CAPP_ASSERT(!_initialized, "Mesh library already initialized!");
		return;
	}
	
	_initialized = true;
}

void MeshLibrary::shutdown() {
	for(const auto& mesh : _meshes) {
		delete mesh.second;
	}
	_meshes.clear();
}

void MeshLibrary::addMesh(const std::string& name, Mesh* mesh) {
	if(hasMesh(name)) {
		CAPP_PRINT_ERROR("Mesh name: {0}", name);
		CAPP_ASSERT(!hasMesh(name), "Mesh already exists!");
		return;
	}

	_meshes[name] = mesh;
}

Mesh* MeshLibrary::loadMesh(const std::string& name, const std::string& filepath) {
	if(hasMesh(name)) {
		CAPP_PRINT_ERROR("Mesh name: {0}", name);
		CAPP_ASSERT(!hasMesh(name), "Mesh already exists!");
		return _meshes[name];
	}

	return _meshes[name] = new Mesh(name, filepath);
}

Mesh* MeshLibrary::getMesh(const std::string& name) {
	if(!hasMesh(name)) {
		CAPP_PRINT_ERROR("Mesh name: {0}", name);
		CAPP_ASSERT(hasMesh(name), "Mesh not found!");
		return nullptr;
	}

	return _meshes[name];
}

bool MeshLibrary::hasMesh(const std::string& name) { return _meshes.find(name) != _meshes.end(); }