#pragma once

#include "Cappuccino/Rendering/Buffers.h"

namespace Capp {

	class VertexArray {
	public:

		VertexArray();
		~VertexArray();

		void bind() const;
		static void unbind();
		
		void addVertexBuffer(VertexBuffer* vertexBuffer);
		void setIndexBuffer(IndexBuffer* indexBuffer);

		const std::vector<VertexBuffer*>& getVertexBuffers() const;
		const IndexBuffer* getIndexBuffer() const;

	private:

		unsigned int _id = 0;
		std::vector<VertexBuffer*> _vertexBuffers;
		IndexBuffer* _indexBuffer;
		
	};
	
}
