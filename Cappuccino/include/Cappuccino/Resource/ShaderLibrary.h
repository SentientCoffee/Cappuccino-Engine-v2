#pragma once

#include "Cappuccino/Rendering/Shaders/Shader.h"

#include <string>
#include <unordered_map>

namespace Capp {

	class ShaderLibrary {
	public:

		static void init();
		static void shutdown();

		static void addShader(const std::string& name, Shader* shader);
		static Shader* loadShader(const std::string& name);
		static Shader* getShader(const std::string& name);
		
	private:

		static bool hasShader(const std::string& name);
		
		static std::unordered_map<std::string, Shader*> _shaders;
		static bool _initialized;
		
	};
	
}
