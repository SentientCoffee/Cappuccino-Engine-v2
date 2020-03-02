#pragma once

namespace Capp {
	
	template<SDT Val, typename S>
	void Shader::setUniform(const std::string& uniformName, const S& value) {
		CAPP_ASSERT(false, "Invalid shader data type to send to uniform!");
	}

	template<>
	inline void Shader::setUniform<Bool>(const std::string& uniformName, const bool& value) {
		setUniformBool(uniformName, value);
	}

	template<>
	inline void Shader::setUniform<Int>(const std::string& uniformName, const int& value) {
		setUniformInt(uniformName, value);
	}

	template<>
	inline void Shader::setUniform<Float>(const std::string& uniformName, const float& value) {
		setUniformFloat(uniformName, value);
	}

	template<>
	inline void Shader::setUniform<Vec2>(const std::string& uniformName, const glm::vec2& value) {
		setUniformVec2(uniformName, value);
	}

	template<>
	inline void Shader::setUniform<Vec3>(const std::string& uniformName, const glm::vec3& value) {
		setUniformVec3(uniformName, value);
	}

	template<>
	inline void Shader::setUniform<Bool>(const std::string& uniformName, const glm::vec4& value) {
		setUniformVec4(uniformName, value);
	}

	template<>
	inline void Shader::setUniform<Bool>(const std::string& uniformName, const glm::mat3& value) {
		setUniformMat3(uniformName, value);
	}

	template<>
	inline void Shader::setUniform<Bool>(const std::string& uniformName, const glm::mat4& value) {
		setUniformMat4(uniformName, value);
	}
	
}