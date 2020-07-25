#pragma once

namespace Capp {
	
	template<SDT Val, typename S>
	void Shader::setUniform(const std::string& uniformName, const S& value) {
		CAPP_ASSERT(false, "Invalid shader data type to send to uniform!\n\tUniform: {0}", uniformName);
	}

	template<SDT Val, typename S>
	void Shader::setUniformArray(const std::string& uniformName, uint32_t count, const S* values) {
		CAPP_ASSERT(false, "Invalid shader data type to send to uniform array!\n\tUniform array: {0}", uniformName);
	}

	template<SDT Val, typename S>
	void Shader::setUniformArray(const std::string& uniformName, const std::vector<S>& values) {
		CAPP_ASSERT(false, "Invalid shader data type to send to uniform array!\n\tUniform array: {0}", uniformName);
	}

	template<SDT Val, typename S, size_t Size>
	void Shader::setUniformArray(const std::string& uniformName, const std::array<S, Size>& values) {
		setUniformArray<Val>(uniformName, Size, values.data());
	}

	// ---------------------------------------------------------------------------------------------
	// ---- Shader uniforms ------------------------------------------------------------------------
	// ---------------------------------------------------------------------------------------------
	
	template<> inline void Shader::setUniform<Bool> (const std::string& uniformName, const bool& value)      { setUniformBool (uniformName, value); }
	template<> inline void Shader::setUniform<Int>  (const std::string& uniformName, const int& value)       { setUniformInt  (uniformName, value); }
	template<> inline void Shader::setUniform<Float>(const std::string& uniformName, const float& value)     { setUniformFloat(uniformName, value); }
	template<> inline void Shader::setUniform<Vec2> (const std::string& uniformName, const glm::vec2& value) { setUniformVec2 (uniformName, value); }
	template<> inline void Shader::setUniform<Vec3> (const std::string& uniformName, const glm::vec3& value) { setUniformVec3 (uniformName, value); }
	template<> inline void Shader::setUniform<Vec4> (const std::string& uniformName, const glm::vec4& value) { setUniformVec4 (uniformName, value); }
	template<> inline void Shader::setUniform<Mat3> (const std::string& uniformName, const glm::mat3& value) { setUniformMat3 (uniformName, value); }
	template<> inline void Shader::setUniform<Mat4> (const std::string& uniformName, const glm::mat4& value) { setUniformMat4 (uniformName, value); }

	// ---------------------------------------------------------------------------------------------
	// ---- Shader uniform C-arrays ----------------------------------------------------------------
	// ---------------------------------------------------------------------------------------------

	template<> inline void Shader::setUniformArray<Bool> (const std::string& uniformName, const uint32_t count, const bool* values)      { setUniformBoolArray (uniformName, count, values); }
	template<> inline void Shader::setUniformArray<Int>  (const std::string& uniformName, const uint32_t count, const int* values)       { setUniformIntArray  (uniformName, count, values); }
	template<> inline void Shader::setUniformArray<Float>(const std::string& uniformName, const uint32_t count, const float* values)     { setUniformFloatArray(uniformName, count, values); }
	template<> inline void Shader::setUniformArray<Vec2> (const std::string& uniformName, const uint32_t count, const glm::vec2* values) { setUniformVec2Array (uniformName, count, values); }
	template<> inline void Shader::setUniformArray<Vec3> (const std::string& uniformName, const uint32_t count, const glm::vec3* values) { setUniformVec3Array (uniformName, count, values); }
	template<> inline void Shader::setUniformArray<Vec4> (const std::string& uniformName, const uint32_t count, const glm::vec4* values) { setUniformVec4Array (uniformName, count, values); }
	template<> inline void Shader::setUniformArray<Mat3> (const std::string& uniformName, const uint32_t count, const glm::mat3* values) { setUniformMat3Array (uniformName, count, values); }
	template<> inline void Shader::setUniformArray<Mat4> (const std::string& uniformName, const uint32_t count, const glm::mat4* values) { setUniformMat4Array (uniformName, count, values); }

	// ---------------------------------------------------------------------------------------------
	// ---- Shader uniform std::vectors ------------------------------------------------------------
	// ---------------------------------------------------------------------------------------------

	template<> inline void Shader::setUniformArray<Bool> (const std::string& uniformName, const std::vector<bool>& values)      { CAPP_ASSERT(false, "Cannot use std::vector<bool> because of STL implementations, please use std::array or a bool[]"); }
	template<> inline void Shader::setUniformArray<Int>  (const std::string& uniformName, const std::vector<int>& values)       { setUniformIntArray  (uniformName, static_cast<uint32_t>(values.size()), values.data()); }
	template<> inline void Shader::setUniformArray<Float>(const std::string& uniformName, const std::vector<float>& values)     { setUniformFloatArray(uniformName, static_cast<uint32_t>(values.size()), values.data()); }
	template<> inline void Shader::setUniformArray<Vec2> (const std::string& uniformName, const std::vector<glm::vec2>& values) { setUniformVec2Array (uniformName, static_cast<uint32_t>(values.size()), values.data()); }
	template<> inline void Shader::setUniformArray<Vec3> (const std::string& uniformName, const std::vector<glm::vec3>& values) { setUniformVec3Array (uniformName, static_cast<uint32_t>(values.size()), values.data()); }
	template<> inline void Shader::setUniformArray<Vec4> (const std::string& uniformName, const std::vector<glm::vec4>& values) { setUniformVec4Array (uniformName, static_cast<uint32_t>(values.size()), values.data()); }
	template<> inline void Shader::setUniformArray<Mat3> (const std::string& uniformName, const std::vector<glm::mat3>& values) { setUniformMat3Array (uniformName, static_cast<uint32_t>(values.size()), values.data()); }
	template<> inline void Shader::setUniformArray<Mat4> (const std::string& uniformName, const std::vector<glm::mat4>& values) { setUniformMat4Array (uniformName, static_cast<uint32_t>(values.size()), values.data()); }
	
}