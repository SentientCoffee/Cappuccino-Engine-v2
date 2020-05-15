#pragma once

#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Rendering/Buffers/Buffers.h"

namespace Capp {

	class VertexArray {

		using VertexBufferList = std::vector<Ref<VertexBuffer>>;
		
	public:

		VertexArray();
		~VertexArray();

		static Ref<VertexArray> create() {
			return Memory::createRef<VertexArray>();
		}
		
		void bind() const;
		static void unbind();
		
		void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
		void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

		const VertexBufferList& getVertexBuffers() const { return _vertexBuffers; }
		Ref<IndexBuffer> getIndexBuffer() const { return _indexBuffer; }

		
	private:

		unsigned int _id = 0;
		VertexBufferList _vertexBuffers;
		Ref<IndexBuffer> _indexBuffer;
		
	};
	
}
