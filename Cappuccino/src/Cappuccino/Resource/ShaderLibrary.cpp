#include "CappPCH.h"
#include "Cappuccino/Resource/ShaderLibrary.h"

using namespace Capp;

ShaderLibrary::ShaderMap ShaderLibrary::_shaders;
bool ShaderLibrary::_initialized = false;

void ShaderLibrary::init() {
	if(_initialized) {
		CAPP_ASSERT(!_initialized, "Shader library already initialized!");
		return;
	}

	_initialized = true;
}

void ShaderLibrary::shutdown() {
	_shaders.clear();
}

void ShaderLibrary::addShader(const std::string& name, const Ref<Shader>& shader) {
	if(hasShader(name)) {
		CAPP_ASSERT(!hasShader(name), "Shader \"{0}\" already exists!", name);
		return;
	}

	_shaders[name] = shader;
}

Ref<Shader> ShaderLibrary::loadShader(const std::string& name) {
	if(hasShader(name)) {
		CAPP_PRINT_INFO("Shader \"{0}\" already exists, using loaded shader...", name);
		return _shaders[name];
	}

	return _shaders[name] = Shader::create(name);
}

Ref<Shader> ShaderLibrary::getShader(const std::string& name) {
	if(!hasShader(name)) {
		CAPP_ASSERT(hasShader(name), "Shader \"{0}\" not found!", name);
		return nullptr;
	}

	return _shaders[name];
}

bool ShaderLibrary::hasShader(const std::string& name) { return _shaders.find(name) != _shaders.end(); }
