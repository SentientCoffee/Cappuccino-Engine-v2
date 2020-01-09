#pragma once

#include <string>
#include <optional>

namespace Capp {

	enum class ShaderType : unsigned int {
		Vertex = 0,
		Fragment,
		Geometry
	};

	class Shader {
	public:

		Shader() = default;
		Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::optional<std::string>& geometryPath = std::nullopt);
		~Shader();

		void bind() const;
		static void unbind();

	private:

		void compileProgram(unsigned int vertShader, unsigned int fragShader, const std::optional<unsigned>& geomShader = std::nullopt) const;

		static unsigned int createShader(const std::string& shaderSrc, ShaderType shaderType);

		unsigned int _rendererId;
		std::string _vertexSrcPath;
		std::string _fragmentSrcPath;
		std::string _geometrySrcPath;
		
	};
	
}