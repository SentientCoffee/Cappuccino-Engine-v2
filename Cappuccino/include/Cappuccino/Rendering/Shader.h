#pragma once

#include <glm/glm.hpp>

#include <string>
#include <optional>

namespace Capp {

	class Shader {
	public:

		Shader() = default;
		Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::optional<std::string>& geometryPath = std::nullopt);
		~Shader();

		void bind() const;
		static void unbind();

		void reload();
		
		void setName(const std::string& name);
		const std::string& getName() const;

		unsigned getRendererID() const;

		void setUniform(const std::string& uniformName, bool value) const;
		void setUniform(const std::string& uniformName, int value) const;
		void setUniform(const std::string& uniformName, float value) const;
		
		void setUniform(const std::string& uniformName, const glm::vec2& value) const;
		void setUniform(const std::string& uniformName, const glm::vec3& value) const;
		void setUniform(const std::string& uniformName, const glm::vec4& value) const;

		void setUniform(const std::string& uniformName, const glm::mat3& value) const;
		void setUniform(const std::string& uniformName, const glm::mat4& value) const;

	private:
		
		enum class ShaderType : unsigned int {
			Vertex = 0,
			Fragment,
			Geometry
		};
		
		unsigned compileProgram(unsigned int vertShader, unsigned int fragShader, const std::optional<unsigned>& geomShader = std::nullopt) const;
		unsigned createShader(const std::string& shaderSrc, ShaderType shaderType) const;

		int getUniformLocation(const std::string& uniformName) const;
		
		unsigned int _id = 0;
		std::string _name;
		std::string _vertexSrcPath;
		std::string _fragmentSrcPath;
		std::string _geometrySrcPath;
		
	};
	
}