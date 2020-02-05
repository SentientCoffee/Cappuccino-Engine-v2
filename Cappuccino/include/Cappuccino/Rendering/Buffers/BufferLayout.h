#pragma once

#include <string>
#include <vector>

namespace Capp {

	enum class ShaderDataType : unsigned int {
		Bool = 0, Int, Float,
		Vec2, Vec3, Vec4,
		Mat3, Mat4
	};

	struct BufferElement {
		std::string name;
		ShaderDataType type;
		unsigned size;
		unsigned offset;
		bool normalized;

		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false);

		unsigned int getComponentCount() const;
	};

	class BufferLayout {

		using BufferElemVector = std::vector<BufferElement>;
		using BufferElemInitList = std::initializer_list<BufferElement>;
		using BufferElemIterator = std::vector<BufferElement>::iterator;
		using ConstBufferElemIterator = std::vector<BufferElement>::const_iterator;

	public:

		BufferLayout() = default;
		BufferLayout(const BufferElemVector& elements);
		BufferLayout(const BufferElemInitList& elements);
		~BufferLayout();

		BufferElemVector getElements() const;
		unsigned int size() const;
		
		BufferElemIterator begin();
		BufferElemIterator end();
		ConstBufferElemIterator begin() const;
		ConstBufferElemIterator end() const;

		unsigned int getStride() const;

	private:

		void strideOffsetCalc();

		BufferElemVector _elements;
		unsigned int _stride = 0;
	};
	
}