#include "CappPCH.h"
#include "Cappuccino/Rendering/BufferLayout.h"

using namespace Capp;

static unsigned int SDTypeSize(const ShaderDataType type) {
	switch(type) {
		case ShaderDataType::Bool:		return sizeof(int);
		case ShaderDataType::Int:		return sizeof(int);
		case ShaderDataType::Float:		return sizeof(float);

		case ShaderDataType::Vec2:		return 2 * sizeof(float);
		case ShaderDataType::Vec3:		return 3 * sizeof(float);
		case ShaderDataType::Vec4:		return 4 * sizeof(float);

		case ShaderDataType::Mat3:		return 3 * 3 * sizeof(float);
		case ShaderDataType::Mat4:		return 4 * 4 * sizeof(float);
	}

	CAPP_ASSERT(false, "Unknown shader data type!");
	return 0;
}

// ----------------------------------------
// ----- Buffer layout element ------------
// ----------------------------------------

BufferElement::BufferElement(const ShaderDataType type, const std::string& name, const bool normalized) :
	name(name), type(type), size(SDTypeSize(type)), offset(0), normalized(normalized) {}

unsigned BufferElement::getComponentCount() const {
	switch(type) {
		case ShaderDataType::Bool:		return 1;
		case ShaderDataType::Int:		return 1;
		case ShaderDataType::Float:		return 1;

		case ShaderDataType::Vec2:		return 2;
		case ShaderDataType::Vec3:		return 3;
		case ShaderDataType::Vec4:		return 4;

		case ShaderDataType::Mat3:		return 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4;
	}

	return 0;
}

// ----------------------------------------
// ----- Buffer layout --------------------
// ----------------------------------------

BufferLayout::BufferLayout(const std::vector<BufferElement>& elements) :
	_elements(elements) {
	strideOffsetCalc();
}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements) :
	_elements(elements) {
	strideOffsetCalc();
}

BufferLayout::~BufferLayout() {
	_elements.clear();
}

std::vector<BufferElement> BufferLayout::getElements() const { return _elements; }
unsigned int BufferLayout::size() const { return static_cast<unsigned int>(_elements.size()); }

BufferLayout::BufferElemIterator BufferLayout::begin() { return _elements.begin(); }
BufferLayout::BufferElemIterator BufferLayout::end() { return _elements.end(); }
BufferLayout::ConstBufferElemIterator BufferLayout::begin() const { return _elements.begin(); }
BufferLayout::ConstBufferElemIterator BufferLayout::end() const { return _elements.end(); }

unsigned BufferLayout::getStride() const { return _stride; }

void BufferLayout::strideOffsetCalc() {
	unsigned offset = _stride = 0;

	for(auto& elem : _elements) {
		elem.offset = offset;
		offset += elem.size;
		_stride += elem.size;
	}
}