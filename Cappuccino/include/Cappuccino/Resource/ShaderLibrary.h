#pragma once

#include "Cappuccino/Rendering/Shaders/Shader.h"

#include <string>
#include <unordered_map>

namespace Capp {

	class ShaderLibrary {

		using ShaderMap = std::unordered_map<std::string, Ref<Shader>>;
		
	public:

		static void init();
		static void shutdown();

		static void addShader(const std::string& name, const Ref<Shader>& shader);
		static Ref<Shader> loadShader(const std::string& name);
		static Ref<Shader> getShader(const std::string& name);
		
	private:

		static bool hasShader(const std::string& name);
		
		static ShaderMap _shaders;
		static bool _initialized;
		
	};
	
}
