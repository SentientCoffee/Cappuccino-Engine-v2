#include "CappPCH.h"
#include "Cappuccino/Rendering/Shader.h"

#include <glad/glad.h>

using namespace Capp;

std::string loadFileAsString(const std::string& filepath) {

	const std::ifstream inStream(filepath.data());
	std::stringstream fileContent;

	if(!inStream.good()) {
		CAPP_PRINT_CRITICAL("File: {0}", filepath);
		CAPP_ASSERT(false, "File could not be read!");
	}
	
	fileContent << inStream.rdbuf();
	return fileContent.str();
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::optional<std::string>& geometryPath) {
	_rendererId = glCreateProgram();

	_vertexSrcPath = vertexPath;
	_fragmentSrcPath = vertexPath;
	
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
		
		const std::string& geometrySrc = loadFileAsString(vertexPath);
		CAPP_ASSERT(!geometrySrc.empty(), "Geometry shader could not be read!");
		
		const unsigned int geomShader = createShader(geometrySrc, ShaderType::Fragment);
		compileProgram(vertShader, fragShader, geomShader);
	}
	else {
		compileProgram(vertShader, fragShader);
	}
}

Shader::~Shader() { glDeleteProgram(_rendererId); }

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
		switch(shaderType) {
			case ShaderType::Vertex:
				type = "Vertex";
				break;
			case ShaderType::Fragment:
				type = "Fragment";
				break;
			case ShaderType::Geometry:
				type = "Geometry";
				break;
		}

		CAPP_PRINT_CRITICAL("{0} shader:", type);
		CAPP_PRINT_CRITICAL("{0}", infoLog.data());
		CAPP_ASSERT(false, "Failed to compile shader!");
		return 0;
	}

	return shaderHandle;
}

void Shader::compileProgram(const unsigned int vertShader, const unsigned int fragShader, const std::optional<unsigned>& geomShader) const {
	CAPP_ASSERT(vertShader, "No vertex shader linked!");
	CAPP_ASSERT(fragShader, "No fragment shader linked!");

	glAttachShader(_rendererId, vertShader);
	glAttachShader(_rendererId, fragShader);

	if(geomShader) {
		CAPP_ASSERT(!geomShader.value(), "No geometry shader linked!");
		glAttachShader(_rendererId, geomShader.value());
	}

	glLinkProgram(_rendererId);

	int success;
	glGetProgramiv(_rendererId, GL_LINK_STATUS, &success);
	if(!success) {
		int logLength;
		glGetProgramiv(_rendererId, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> infoLog(logLength);
		glGetProgramInfoLog(_rendererId, logLength, &logLength, infoLog.data());

		glDeleteProgram(_rendererId);
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		if(geomShader) {
			glDeleteShader(geomShader.value());
		}

		CAPP_ASSERT(success, "Failed to link shader program!\n{0}", infoLog.data());
	}

	glDetachShader(_rendererId, vertShader);
	glDetachShader(_rendererId, fragShader);
	if(geomShader) {
		glDetachShader(_rendererId, geomShader.value());
	}
}


void Shader::bind() const { glUseProgram(_rendererId); }
void Shader::unbind() { glUseProgram(0); }