#include "CappPCH.h"
#include "Cappuccino/Rendering/Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

using namespace Capp;

std::string loadShaderAsString(const std::string& filepath) {

	std::ifstream file(filepath.data());
	std::stringstream fileContent;
	CAPP_ASSERT(fileContent.good(), "File could not be read!\nShader file: {0}", filepath);
	
	fileContent << file.rdbuf();
	file.close();
	
	return fileContent.str();
}

Shader::Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::optional<std::string>& geometryPath) {
	_name = name;
	
	_vertexSrcPath = vertexPath;
	_fragmentSrcPath = fragmentPath;
	
	CAPP_ASSERT(!vertexPath.empty(), "Invalid vertex shader file path!\nShader file: {0}", vertexPath);
	CAPP_ASSERT(!fragmentPath.empty(), "Invalid fragment shader file path!\nShader file: {0}", fragmentPath);
	
	const std::string vertexSrc = loadShaderAsString(vertexPath);
	const std::string fragmentSrc = loadShaderAsString(fragmentPath);
	
	CAPP_ASSERT(!vertexSrc.empty(), "Vertex shader could not be read!\nShader file: {0}", vertexPath);
	CAPP_ASSERT(!fragmentSrc.empty(), "Fragment shader could not be read!\nShader file: {0}", fragmentPath);

	const unsigned int vertShader = createShader(vertexSrc, ShaderType::Vertex);
	const unsigned int fragShader = createShader(fragmentSrc, ShaderType::Fragment);

	if(geometryPath) {
		CAPP_ASSERT(!geometryPath.value().empty(), "Invalid geometry shader file path!\nShader file: {0}", geometryPath.value());
		_geometrySrcPath = geometryPath.value();
		
		const std::string& geometrySrc = loadShaderAsString(geometryPath.value());
		CAPP_ASSERT(!geometrySrc.empty(), "Geometry shader could not be read!\nShader file: {0}", geometryPath.value());
		
		const unsigned int geomShader = createShader(geometrySrc, ShaderType::Geometry);
		_id = compileProgram(vertShader, fragShader, geomShader);
	}
	else {
		_id = compileProgram(vertShader, fragShader);
	}
}

Shader::~Shader() { glDeleteProgram(_id); }

void Shader::reload() {
	CAPP_ASSERT(!_vertexSrcPath.empty(), "Invalid vertex shader file path!\nShader file: {0}", _vertexSrcPath);
	CAPP_ASSERT(!_fragmentSrcPath.empty(), "Invalid fragment shader file path!\nShader file: {0}", _fragmentSrcPath);
	
	const std::string vertexSrc = loadShaderAsString(_vertexSrcPath);
	const std::string fragmentSrc = loadShaderAsString(_fragmentSrcPath);

	CAPP_ASSERT(!vertexSrc.empty(), "Vertex shader could not be read!\nShader file: {0}", vertexSrc);
	CAPP_ASSERT(!fragmentSrc.empty(), "Fragment shader could not be read!\nShader file: {0}", fragmentSrc);

	const unsigned int vertShader = createShader(vertexSrc, ShaderType::Vertex);
	const unsigned int fragShader = createShader(fragmentSrc, ShaderType::Fragment);

	const unsigned newProgram = compileProgram(vertShader, fragShader);
	CAPP_ASSERT(newProgram != 0, "Could not reload shader \"{0}\"!", _name);
	_id = newProgram;
}

unsigned Shader::createShader(const std::string& shaderSrc, const ShaderType shaderType) const {
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
		
		CAPP_ASSERT(success, "Failed to compile shader!\n{0} shader at {1}:\n{2}", type, path, infoLog.data());
		return 0;
	}

	return shaderHandle;
}

unsigned Shader::compileProgram(const unsigned int vertShader, const unsigned int fragShader, const std::optional<unsigned>& geomShader) const {
	const unsigned programHandle = glCreateProgram();

	CAPP_ASSERT(vertShader, "No vertex shader linked!");
	CAPP_ASSERT(fragShader, "No fragment shader linked!");

	glAttachShader(programHandle, vertShader);
	glAttachShader(programHandle, fragShader);

	if(geomShader) {
		CAPP_ASSERT(!geomShader.value(), "No geometry shader linked!");
		glAttachShader(programHandle, geomShader.value());
	}

	glLinkProgram(programHandle);

	int success;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &success);
	if(!success) {
		int logLength;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> infoLog(logLength);
		glGetProgramInfoLog(programHandle, logLength, &logLength, infoLog.data());

		glDeleteProgram(programHandle);
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		if(geomShader) {
			glDeleteShader(geomShader.value());
		}

		CAPP_ASSERT(success, "Failed to link shader program!\nShader: {0}\n{1}", _name, infoLog.data());
		return 0;
	}

	glDetachShader(programHandle, vertShader);
	glDetachShader(programHandle, fragShader);
	if(geomShader) {
		glDetachShader(programHandle, geomShader.value());
	}

	return programHandle;
}


void Shader::bind() const { glUseProgram(_id); }
void Shader::unbind() { glUseProgram(0); }

void Shader::setName(const std::string& name) { _name = name; }
const std::string& Shader::getName() const { return _name; }

int Shader::getUniformLocation(const std::string& uniformName) const {
	const int uniformLocation = glGetUniformLocation(_id, uniformName.c_str());
	CAPP_ASSERT(uniformLocation != -1, "Could not find uniform \"{0}\" in shader \"{1}\"", uniformName, _name);
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
