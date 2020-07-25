#pragma once

#include "Cappuccino/Core/Memory.h"
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

		static Ref<Shader> create(const std::string& name) {
			return Memory::createRef<Shader>(name);
		}
		
		void attach(const std::string& filepath, ShaderStage stage);
		void compile();

		void bind() const;
		static void unbind();

		void reload();
		
		void setName(const std::string& name) { _name = name; }
		const std::string& getName() const { return _name; }

		uint32_t getRendererID() const { return _id; }

		template<SDT Val, typename S>
		void setUniform(const std::string& uniformName, const S& value);

		template<SDT Val, typename S>
		void setUniformArray(const std::string& uniformName, uint32_t count, const S* values);

		template<SDT Val, typename S>
		void setUniformArray(const std::string& uniformName, const std::vector<S>& values);

		template<SDT Val, typename S, size_t Size>
		void setUniformArray(const std::string& uniformName, const std::array<S, Size>& values);
		
	private:

		uint32_t compileProgram() const;
		uint32_t createShader(const std::string& shaderSrc, ShaderStage stage);

		int getUniformLocation(const std::string& uniformName) const;
		
		void setUniformBool(const std::string& uniformName, const bool& value) const;
		void setUniformInt(const std::string& uniformName, const int& value) const;
		void setUniformFloat(const std::string& uniformName, const float& value) const;

		void setUniformVec2(const std::string& uniformName, const glm::vec2& value) const;
		void setUniformVec3(const std::string& uniformName, const glm::vec3& value) const;
		void setUniformVec4(const std::string& uniformName, const glm::vec4& value) const;

		void setUniformMat3(const std::string& uniformName, const glm::mat3& value) const;
		void setUniformMat4(const std::string& uniformName, const glm::mat4& value) const;

		void setUniformBoolArray(const std::string& uniformName, uint32_t count, const bool* values) const;
		void setUniformIntArray(const std::string& uniformName, uint32_t count, const int* values) const;
		void setUniformFloatArray(const std::string& uniformName, uint32_t count, const float* values) const;

		void setUniformVec2Array(const std::string& uniformName, uint32_t count, const glm::vec2* values) const;
		void setUniformVec3Array(const std::string& uniformName, uint32_t count, const glm::vec3* values) const;
		void setUniformVec4Array(const std::string& uniformName, uint32_t count, const glm::vec4* values) const;
		
		void setUniformMat3Array(const std::string& uniformName, uint32_t count, const glm::mat3* values) const;
		void setUniformMat4Array(const std::string& uniformName, uint32_t count, const glm::mat4* values) const;
		
		uint32_t _id = 0;
		std::string _name;
		bool _isCompiled = false;
		
		std::unordered_map<ShaderStage, std::string> _filepaths;
		std::map<ShaderStage, uint32_t> _handles;
	};
	
}

#include "ShaderUniforms.inl"