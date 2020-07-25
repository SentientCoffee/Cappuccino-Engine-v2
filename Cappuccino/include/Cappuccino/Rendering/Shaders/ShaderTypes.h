#pragma once

namespace Capp {
	
	enum class ShaderDataType : uint32_t {
		Bool = 0, Int, Float,
		Vec2, Vec3, Vec4,
		Mat3, Mat4
	};

	enum class ShaderStage : uint32_t {
		Vertex = 0,
		TessControl,
		TessEvaluation,
		Geometry,
		Fragment,
		Compute
	};
	
#if CAPP_DEBUG || CAPP_RELEASE
	inline std::string enumToString(const ShaderDataType& dataType) {
		switch(dataType) {
			case ShaderDataType::Bool:		return "Bool";
			case ShaderDataType::Int:		return "Int";
			case ShaderDataType::Float:		return "Float";
			case ShaderDataType::Vec2:		return "Vec2";
			case ShaderDataType::Vec3:		return "Vec3";
			case ShaderDataType::Vec4:		return "Vec4";
			case ShaderDataType::Mat3:		return "Mat3";
			case ShaderDataType::Mat4:		return "Mat4";
			default:						return "Unknown ShaderDataType type!";
		}
	}

	inline std::ostream& operator<<(std::ostream& out, const ShaderDataType& type) { return out << enumToString(type); }
	
	inline std::string enumToString(const ShaderStage& stage) {
		switch(stage) {
			case ShaderStage::Vertex:				return "Vertex";
			case ShaderStage::TessControl:			return "TessControl";
			case ShaderStage::TessEvaluation:		return "TessEvaluation";
			case ShaderStage::Geometry:				return "Geometry";
			case ShaderStage::Fragment:				return "Fragment";
			case ShaderStage::Compute:				return "Compute";
			default:								return "Unknown ShaderStage type!";
		}
	}

	inline std::ostream& operator<<(std::ostream& out, const ShaderStage& type) { return out << enumToString(type); }
#endif
	
}

enum SDT {
	Bool  = Capp::ShaderDataType::Bool,
	Int   = Capp::ShaderDataType::Int,
	Float = Capp::ShaderDataType::Float,
	Vec2  = Capp::ShaderDataType::Vec2,
	Vec3  = Capp::ShaderDataType::Vec3,
	Vec4  = Capp::ShaderDataType::Vec4,
	Mat3  = Capp::ShaderDataType::Mat3,
	Mat4  = Capp::ShaderDataType::Mat4
};