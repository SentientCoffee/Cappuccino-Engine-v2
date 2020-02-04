#include "CappPCH.h"
#include "Cappuccino/Resource/ShaderLibrary.h"

using namespace Capp;

std::unordered_map<std::string, Shader*> ShaderLibrary::_shaders;
bool ShaderLibrary::_initialized = false;


void ShaderLibrary::init() {
	if(_initialized) {
		CAPP_ASSERT(!_initialized, "Shader library already initialized!");
		return;
	}

	_initialized = true;
}

void ShaderLibrary::shutdown() {
	for(const auto& shader : _shaders) {
		delete shader.second;
	}
	_shaders.clear();
}

void ShaderLibrary::addShader(const std::string& name, Shader* shader) {
	if(hasShader(name)) {
		CAPP_ASSERT(!hasShader(name), "Shader \"{0}\" already exists!", name);
		return;
	}

	_shaders[name] = shader;
}

Shader* ShaderLibrary::loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::optional<std::string>& geometryPath) {
	if(hasShader(name)) {
		CAPP_PRINT_INFO("Shader \"{0}\" already exists, using loaded shader...", name);
		return _shaders[name];
	}

	return _shaders[name] = new Shader(name, vertexPath, fragmentPath, geometryPath);
}

Shader* ShaderLibrary::getShader(const std::string& name) {
	if(!hasShader(name)) {
		CAPP_ASSERT(hasShader(name), "Shader \"{0}\" not found!", name);
		return nullptr;
	}

	return _shaders[name];
}

bool ShaderLibrary::hasShader(const std::string& name) { return _shaders.find(name) != _shaders.end(); }
