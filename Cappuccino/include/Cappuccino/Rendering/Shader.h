#pragma once

#include <string>
#include <optional>

namespace Capp {

	class Shader {
	public:

		Shader() = default;
		Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::optional<std::string>& geometryPath = std::nullopt);
		~Shader();

		void bind() const;
		static void unbind();

	private:
		
		enum class ShaderType : unsigned int {
			Vertex = 0,
			Fragment,
			Geometry
		};
		
		void compileProgram(unsigned int vertShader, unsigned int fragShader, const std::optional<unsigned>& geomShader = std::nullopt) const;

		unsigned int createShader(const std::string& shaderSrc, ShaderType shaderType);

		unsigned int _id = 0;
		std::string _vertexSrcPath;
		std::string _fragmentSrcPath;
		std::string _geometrySrcPath;
		
	};
	
}