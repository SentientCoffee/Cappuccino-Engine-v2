#include "CappPCH.h"
#include "Cappuccino/Rendering/Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

using namespace Capp;

std::string loadFileAsString(const std::string& filepath) {

	const std::ifstream inStream(filepath.data());
	std::stringstream fileContent;

	if(!inStream.good()) {
		CAPP_PRINT_ERROR("File: {0}", filepath);
		CAPP_ASSERT(false, "File could not be read!");
	}
	
	fileContent << inStream.rdbuf();
	return fileContent.str();
}

Shader::Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::optional<std::string>& geometryPath) {
	_id = glCreateProgram();
	_name = name;
	
	_vertexSrcPath = vertexPath;
	_fragmentSrcPath = fragmentPath;
	
	CAPP_ASSERT(!vertexPath.empty(), "Invalid vertex shader file path!");
	CAPP_ASSERT(!fragmentPath.empty(), "Invalid fragment shader file path!");
	
	const std::string vertexSrc = loadFileAsString(vertexPath);
	const std::string fragmentSrc = loadFileAsString(fragmentPath);
	
	CAPP_ASSERT(!vertexSrc.empty(), "Vertex shader could not be read!");
	CAPP_ASSERT(!fragmentSrc.empty(), "Fragment shader could not be read!");

	const unsigned int vertShader = createShader(vertexSrc, ShaderType::Vertex);
	const unsigned int fragShader = createShader(fragmentSrc, ShaderType::Fragment);

	if(geometryPath) {
		CAPP_ASSERT(!geometryPath.value().empty(), "Invalid geometry shader file path!");
		_geometrySrcPath = geometryPath.value();
		
		const std::string& geometrySrc = loadFileAsString(geometryPath.value());
		CAPP_ASSERT(!geometrySrc.empty(), "Geometry shader could not be read!");
		
		const unsigned int geomShader = createShader(geometrySrc, ShaderType::Fragment);
		compileProgram(vertShader, fragShader, geomShader);
	}
	else {
		compileProgram(vertShader, fragShader);
	}
}

Shader::~Shader() { glDeleteProgram(_id); }

unsigned Shader::createShader(const std::string& shaderSrc, const ShaderType shaderType) {
	unsigned int shaderHandle = 0;

	switch(shaderType) {
		case ShaderType::Vertex:
			shaderHandle = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderType::Fragment:
			shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case ShaderType::Geometry:
			shaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
			break;
	}

	auto shaderSource = shaderSrc.c_str();
	glShaderSource(shaderHandle, 1, &shaderSource, nullptr);
	glCompileShader(shaderHandle);

	int success;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
	if(!success) {
		int logLength;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> infoLog(logLength);
		glGetShaderInfoLog(shaderHandle, logLength, &logLength, infoLog.data());
		glDeleteShader(shaderHandle);

		std::string type;
		std::string path;
		switch(shaderType) {
			case ShaderType::Vertex:
				type = "Vertex";
				path = _vertexSrcPath;
				break;
			case ShaderType::Fragment:
				type = "Fragment";
				path = _fragmentSrcPath;
				break;
			case ShaderType::Geometry:
				type = "Geometry";
				path = _geometrySrcPath;
				break;
			default:
				type = "Unknown";
				path = "";
				break;
		}

		CAPP_PRINT_ERROR("{0} shader at {1}:\n{2}", type, path, infoLog.data());
		CAPP_ASSERT(false, "Failed to compile shader!");
		return 0;
	}

	return shaderHandle;
}

void Shader::compileProgram(const unsigned int vertShader, const unsigned int fragShader, const std::optional<unsigned>& geomShader) const {
	CAPP_ASSERT(vertShader, "No vertex shader linked!");
	CAPP_ASSERT(fragShader, "No fragment shader linked!");

	glAttachShader(_id, vertShader);
	glAttachShader(_id, fragShader);

	if(geomShader) {
		CAPP_ASSERT(!geomShader.value(), "No geometry shader linked!");
		glAttachShader(_id, geomShader.value());
	}

	glLinkProgram(_id);

	int success;
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if(!success) {
		int logLength;
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> infoLog(logLength);
		glGetProgramInfoLog(_id, logLength, &logLength, infoLog.data());

		glDeleteProgram(_id);
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		if(geomShader) {
			glDeleteShader(geomShader.value());
		}

		CAPP_ASSERT(success, "Failed to link shader program!\n{0}", infoLog.data());
	}

	glDetachShader(_id, vertShader);
	glDetachShader(_id, fragShader);
	if(geomShader) {
		glDetachShader(_id, geomShader.value());
	}
}


void Shader::bind() const { glUseProgram(_id); }
void Shader::unbind() { glUseProgram(0); }

void Shader::setName(const std::string& name) { _name = name; }
const std::string& Shader::getName() const { return _name; }

int Shader::getUniformLocation(const std::string& uniformName) const {
	const int uniformLocation = glGetUniformLocation(_id, uniformName.c_str());
	if(uniformLocation == -1) {
		CAPP_PRINT_ERROR("Uniform \"{0}\" in shader \"{1}\"", uniformName, _name);
		CAPP_ASSERT(!uniformLocation, "Could not find uniform!");
	}
	return uniformLocation;
}

void Shader::setUniform(const std::string& uniformName, const bool value) const {
	glUniform1i(getUniformLocation(uniformName), static_cast<int>(value));
}
void Shader::setUniform(const std::string& uniformName, const int value) const {
	glUniform1i(getUniformLocation(uniformName), value);
}
void Shader::setUniform(const std::string& uniformName, const float value) const {
	glUniform1f(getUniformLocation(uniformName), value);
}
void Shader::setUniform(const std::string& uniformName, const glm::vec2& value) const {
	glUniform2fv(getUniformLocation(uniformName), 1, glm::value_ptr(value));
}
void Shader::setUniform(const std::string& uniformName, const glm::vec3& value) const {
	glUniform3fv(getUniformLocation(uniformName), 1, glm::value_ptr(value));
}
void Shader::setUniform(const std::string& uniformName, const glm::vec4& value) const {
	glUniform4fv(getUniformLocation(uniformName), 1, glm::value_ptr(value));
}
void Shader::setUniform(const std::string& uniformName, const glm::mat3& value) const {
	glUniformMatrix3fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setUniform(const std::string& uniformName, const glm::mat4& value) const {
	glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(value));
}
