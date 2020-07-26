#include "CappPCH.h"
#include "Cappuccino/Rendering/Shaders/Shader.h"
#include "Cappuccino/Resource/AssetLoader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

using namespace Capp;

Shader::Shader(const std::string& name) :
	_name(name) {}

Shader::~Shader() {
	glDeleteProgram(_id);
}

void Shader::attach(const std::string& filepath, const ShaderStage stage) {
	if(_isCompiled) {
		CAPP_ASSERT(!_isCompiled, "Cannot modify shader program after compilation!\n\tShader: {0}", _name);
		return;
	}

	CAPP_ASSERT(!filepath.empty(), "No {0} shader file path given!", stage);

	const std::string shaderSrc = AssetLoader::readTextFile(filepath);
	CAPP_ASSERT(!shaderSrc.empty(), "{0} shader file is empty!\n\tShader file: {1}", stage, filepath);

	const uint32_t shaderHandle = createShader(shaderSrc, stage);

	_filepaths[stage] = filepath;
	_handles[stage] = shaderHandle;
}

void Shader::compile() {
	const uint32_t newProgram = compileProgram();
	CAPP_ASSERT(newProgram != 0, "Could not compile shader \"{0}\"!", _name);
	_id = newProgram;
	_isCompiled = true;
}

void Shader::reload() {
	_isCompiled = false;

	for(const auto& path : _filepaths) {
		attach(path.second, path.first);
	}

	compile();
}

uint32_t Shader::createShader(const std::string& shaderSrc, const ShaderStage stage) {
	uint32_t shaderHandle = 0;

	switch(stage) {
		case ShaderStage::Vertex:
			shaderHandle = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderStage::TessControl:
			shaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
			break;
		case ShaderStage::TessEvaluation:
			shaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
			break;
		case ShaderStage::Geometry:
			shaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		case ShaderStage::Fragment:
			shaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case ShaderStage::Compute:
			shaderHandle = glCreateShader(GL_COMPUTE_SHADER);
			break;
	}

	const auto* shaderSource = shaderSrc.c_str();
	glShaderSource(shaderHandle, 1, &shaderSource, nullptr);
	glCompileShader(shaderHandle);

	int success;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
	if(!success) {
		int logLength;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> infoLog;
		infoLog.reserve(logLength);
		glGetShaderInfoLog(shaderHandle, logLength, &logLength, infoLog.data());
		glDeleteShader(shaderHandle);

		CAPP_PRINT_ERROR("Failed to compile shader!\n\t{0} shader at {1}:\n{2}", stage, _filepaths[stage], infoLog.data());
		return 0;
	}

	return shaderHandle;
}

uint32_t Shader::compileProgram() const {

	const uint32_t programHandle = glCreateProgram();

	for(const auto& handle : _handles) {
		CAPP_ASSERT(handle.second != 0, "No {0} shader linked!\n\tShader: {1}", handle.first, _name);
		glAttachShader(programHandle, handle.second);
	}

	glLinkProgram(programHandle);

	int success;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &success);
	if(!success) {
		int logLength; glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> infoLog; infoLog.reserve(logLength);
		glGetProgramInfoLog(programHandle, logLength, &logLength, infoLog.data());

		glDeleteProgram(programHandle);
		for(const auto& handle : _handles) {
			glDeleteShader(handle.second);
		}

		CAPP_ASSERT(success, "Failed to link shader program!\n\tShader: {0}\n\t{1}", _name, infoLog.data());
		return 0;
	}

	for(const auto& handle : _handles) {
		glDetachShader(programHandle, handle.second);
	}

	return programHandle;
}

void Shader::bind() const {
	CAPP_ASSERT(_isCompiled, "Shader is not compiled!\n\tShader: {0}", _name);
	glUseProgram(_id);
}

void Shader::unbind() {
	glUseProgram(0);
}

int Shader::getUniformLocation(const std::string& uniformName) const {
	// TODO: GET UNIFORM LOCATIONS ON PROGRAM CREATION
	// https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions
	
	const int uniformLocation = glGetUniformLocation(_id, uniformName.c_str());
	if(uniformLocation == -1) {
		CAPP_PRINT_ERROR("Could not find uniform \"{0}\" in shader \"{1}\"", uniformName, _name);
	}
	return uniformLocation;
}

void Shader::setUniformBool (const std::string& uniformName, const bool& value) const      { glUniform1i(getUniformLocation(uniformName), static_cast<int>(value)); }
void Shader::setUniformInt  (const std::string& uniformName, const int& value) const       { glUniform1i(getUniformLocation(uniformName), value); }
void Shader::setUniformFloat(const std::string& uniformName, const float& value) const     { glUniform1f(getUniformLocation(uniformName), value); }
void Shader::setUniformVec2 (const std::string& uniformName, const glm::vec2& value) const { glUniform2fv(getUniformLocation(uniformName), 1, glm::value_ptr(value)); }
void Shader::setUniformVec3 (const std::string& uniformName, const glm::vec3& value) const { glUniform3fv(getUniformLocation(uniformName), 1, glm::value_ptr(value)); }
void Shader::setUniformVec4 (const std::string& uniformName, const glm::vec4& value) const { glUniform4fv(getUniformLocation(uniformName), 1, glm::value_ptr(value)); }
void Shader::setUniformMat3 (const std::string& uniformName, const glm::mat3& value) const { glUniformMatrix3fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(value)); }
void Shader::setUniformMat4 (const std::string& uniformName, const glm::mat4& value) const { glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(value)); }

void Shader::setUniformBoolArray (const std::string& uniformName, const uint32_t count, const bool* values) const      { glUniform1iv(getUniformLocation(uniformName), count, reinterpret_cast<const GLint*>(values)); }
void Shader::setUniformIntArray  (const std::string& uniformName, const uint32_t count, const int* values) const       { glUniform1iv(getUniformLocation(uniformName), count, values); }
void Shader::setUniformFloatArray(const std::string& uniformName, const uint32_t count, const float* values) const     { glUniform1fv(getUniformLocation(uniformName), count, values); }
void Shader::setUniformVec2Array (const std::string& uniformName, const uint32_t count, const glm::vec2* values) const { glUniform2fv(getUniformLocation(uniformName), count, glm::value_ptr(values[0])); }
void Shader::setUniformVec3Array (const std::string& uniformName, const uint32_t count, const glm::vec3* values) const { glUniform3fv(getUniformLocation(uniformName), count, glm::value_ptr(values[0])); }
void Shader::setUniformVec4Array (const std::string& uniformName, const uint32_t count, const glm::vec4* values) const { glUniform4fv(getUniformLocation(uniformName), count, glm::value_ptr(values[0])); }
void Shader::setUniformMat3Array (const std::string& uniformName, const uint32_t count, const glm::mat3* values) const { glUniformMatrix3fv(getUniformLocation(uniformName), count, GL_FALSE, glm::value_ptr(values[0])); }
void Shader::setUniformMat4Array (const std::string& uniformName, const uint32_t count, const glm::mat4* values) const { glUniformMatrix4fv(getUniformLocation(uniformName), count, GL_FALSE, glm::value_ptr(values[0])); }


