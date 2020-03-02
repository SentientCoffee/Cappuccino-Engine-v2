#pragma once

#include "Cappuccino/Rendering/Shaders/ShaderTypes.h"

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>

namespace Capp {

	class Shader {
	public:

		Shader() = default;
		Shader(const std::string& name);
		~Shader();

		void attach(const std::string& filepath, ShaderStage stage);
		void compile();

		void bind() const;
		static void unbind();

		void reload();
		
		void setName(const std::string& name);
		const std::string& getName() const;

		unsigned getRendererID() const;

		template<SDT Val, typename S>
		void setUniform(const std::string& uniformName, const S& value);
		
	private:

		unsigned compileProgram() const;
		unsigned createShader(const std::string& shaderSrc, ShaderStage stage);

		int getUniformLocation(const std::string& uniformName) const;
		
		void setUniformBool(const std::string& uniformName, bool value) const;
		void setUniformInt(const std::string& uniformName, int value) const;
		void setUniformFloat(const std::string& uniformName, float value) const;

		void setUniformVec2(const std::string& uniformName, const glm::vec2& value) const;
		void setUniformVec3(const std::string& uniformName, const glm::vec3& value) const;
		void setUniformVec4(const std::string& uniformName, const glm::vec4& value) const;

		void setUniformMat3(const std::string& uniformName, const glm::mat3& value) const;
		void setUniformMat4(const std::string& uniformName, const glm::mat4& value) const;
		
		unsigned int _id = 0;
		std::string _name;
		
		std::unordered_map<ShaderStage, std::string> _filepaths;
		std::unordered_map<ShaderStage, unsigned int> _handles;
	};
	
}

#include "ShaderUniforms.inl"