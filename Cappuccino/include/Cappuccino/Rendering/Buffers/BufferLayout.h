#pragma once

#include "Cappuccino/Rendering/Shaders/ShaderTypes.h"

#include <string>
#include <vector>

namespace Capp {

	struct BufferElement {
		std::string name;
		ShaderDataType type = ShaderDataType::Float;
		uint32_t size = 0;
		uint32_t offset = 0;
		bool normalized = false;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false);

		uint32_t getComponentCount() const;
	};

	class BufferLayout {

		using BufferElemVector        = std::vector<BufferElement>;
		using BufferElemInitList      = std::initializer_list<BufferElement>;
		using BufferElemIterator      = std::vector<BufferElement>::iterator;
		using ConstBufferElemIterator = std::vector<BufferElement>::const_iterator;

	public:

		BufferLayout() = default;
		BufferLayout(const BufferElemVector& elements) :
			_elements(elements)
		{
			strideOffsetCalc();
		}
		
		BufferLayout(const BufferElemInitList& elements) :
			_elements(elements)
		{
			strideOffsetCalc();
		}

		template<size_t Size> BufferLayout(const std::array<BufferElement, Size>& elements) :
			_elements(elements.begin(), elements.end()) {}

		~BufferLayout() = default;

		BufferElemVector getElements() const { return _elements; }
		size_t size() const { return _elements.size(); }
		
		BufferElemIterator begin() { return _elements.begin(); }
		BufferElemIterator end() { return _elements.end(); }
		ConstBufferElemIterator begin() const { return _elements.begin(); }
		ConstBufferElemIterator end() const { return _elements.end(); }

		uint32_t getStride() const { return _stride; }

	private:

		void strideOffsetCalc();

		BufferElemVector _elements;
		uint32_t _stride = 0;
	};
	
}
