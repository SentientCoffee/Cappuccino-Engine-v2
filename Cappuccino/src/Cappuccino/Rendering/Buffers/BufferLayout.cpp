#include "CappPCH.h"
#include "Cappuccino/Rendering/Buffers/BufferLayout.h"

using namespace Capp;

static uint32_t SDTypeSize(const ShaderDataType type) {
	switch(type) {
		case ShaderDataType::Bool:
		case ShaderDataType::Int:		return sizeof(int);
		case ShaderDataType::Float:		return sizeof(float);

		case ShaderDataType::Vec2:		return 2 * sizeof(float);
		case ShaderDataType::Vec3:		return 3 * sizeof(float);
		case ShaderDataType::Vec4:		return 4 * sizeof(float);

		case ShaderDataType::Mat3:		return 3 * 3 * sizeof(float);
		case ShaderDataType::Mat4:		return 4 * 4 * sizeof(float);
		default:						return 0;
	}
}

// ----------------------------------------
// ----- Buffer layout element ------------
// ----------------------------------------

BufferElement::BufferElement(const ShaderDataType type, const std::string& name, const bool normalized) :
	name(name), type(type), size(SDTypeSize(type)), normalized(normalized) {}

uint32_t BufferElement::getComponentCount() const {
	switch(type) {
		case ShaderDataType::Bool:
		case ShaderDataType::Int:
		case ShaderDataType::Float:		return 1;

		case ShaderDataType::Vec2:		return 2;
		case ShaderDataType::Vec3:		return 3;
		case ShaderDataType::Vec4:		return 4;

		case ShaderDataType::Mat3:		return 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4;
		default:						return 0;
	}
}

// ----------------------------------------
// ----- Buffer layout --------------------
// ----------------------------------------

void BufferLayout::strideOffsetCalc() {
	uint32_t offset = _stride = 0;

	for(auto& elem : _elements) {
		elem.offset = offset;
		offset += elem.size;
		_stride += elem.size;
	}
}