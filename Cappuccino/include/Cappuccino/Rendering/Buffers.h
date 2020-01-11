#pragma once

#include "Cappuccino/Rendering/BufferLayout.h"

namespace Capp {

	class VertexBuffer {
	public:

		VertexBuffer() = default;
		VertexBuffer(const std::vector<float>& vertices);
		~VertexBuffer();

		void bind() const;
		static void unbind();

		unsigned int getCount() const;

		void setLayout(const BufferLayout& layout);
		const BufferLayout& getLayout() const;

	private:

		unsigned _id = 0;
		unsigned _vertexCount = 0;
		BufferLayout _layout;

	};

	class IndexBuffer {
	public:

		IndexBuffer() = default;
		IndexBuffer(const std::vector<unsigned>& indices);
		~IndexBuffer();

		void bind() const;
		static void unbind();

		unsigned int getCount() const;

	private:

		unsigned _id = 0;
		unsigned _indexCount = 0;

	};
	
}
