#pragma once

#include "Cappuccino/Rendering/Shaders/ShaderTypes.h"

#include <string>
#include <vector>

namespace Capp {

	struct BufferElement {
		std::string name;
		ShaderDataType type = ShaderDataType::Float;
		unsigned size = 0;
		unsigned offset = 0;
		bool normalized = false;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false);

		unsigned int getComponentCount() const;
	};

	class BufferLayout {

		using BufferElemVector        = std::vector<BufferElement>;
		using BufferElemInitList      = std::initializer_list<BufferElement>;
		using BufferElemIterator      = std::vector<BufferElement>::iterator;
		using ConstBufferElemIterator = std::vector<BufferElement>::const_iterator;

	public:

		BufferLayout() = default;
		BufferLayout(const BufferElemVector& elements);
		BufferLayout(const BufferElemInitList& elements);
		~BufferLayout();

		BufferElemVector getElements() const { return _elements; }
		unsigned int size() const { return static_cast<unsigned int>(_elements.size()); }
		
		BufferElemIterator begin() { return _elements.begin(); }
		BufferElemIterator end() { return _elements.end(); }
		ConstBufferElemIterator begin() const { return _elements.begin(); }
		ConstBufferElemIterator end() const { return _elements.end(); }

		unsigned int getStride() const { return _stride; }

	private:

		void strideOffsetCalc();

		BufferElemVector _elements;
		unsigned int _stride = 0;
	};
	
}
