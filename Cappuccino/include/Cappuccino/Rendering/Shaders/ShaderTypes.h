#pragma once

namespace Capp {
	
	enum class ShaderDataType : unsigned int {
		Bool = 0,
		Int,
		Float,
		Vec2,
		Vec3,
		Vec4,
		Mat3,
		Mat4
	};

	enum SDT {
		Bool  = ShaderDataType::Bool,
		Int   = ShaderDataType::Int,
		Float = ShaderDataType::Float,
		Vec2  = ShaderDataType::Vec2,
		Vec3  = ShaderDataType::Vec3,
		Vec4  = ShaderDataType::Vec4,
		Mat3  = ShaderDataType::Mat3,
		Mat4  = ShaderDataType::Mat4
	};

	enum class ShaderStage : unsigned int {
		Vertex = 0,
		TessControl,
		TessEvaluation,
		Geometry,
		Fragment,
		Compute
	};
	
	#if CAPP_DEBUG
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