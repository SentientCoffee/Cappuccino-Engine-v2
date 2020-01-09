#include "CappPCH.h"
#include "Cappuccino/Rendering/VertexArray.h"

#include <glad/glad.h>

using namespace Capp;

static GLenum SDTypeToOpenGLType(const ShaderDataType type) {
	switch(type) {
		case ShaderDataType::Bool:		return GL_BOOL;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Float:		return GL_FLOAT;

		case ShaderDataType::Vec2:		return GL_FLOAT;
		case ShaderDataType::Vec3:		return GL_FLOAT;
		case ShaderDataType::Vec4:		return GL_FLOAT;

		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
	}

	CAPP_ASSERT(false, "Unknown shader data type!");
	return 0;
}

VertexArray::VertexArray() :
	_indexBuffer(nullptr) {
	glCreateVertexArrays(1, &_rendererId);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &_rendererId);
	
	for(auto buffer : _vertexBuffers) {
		delete buffer;
	}
	_vertexBuffers.clear();

	delete _indexBuffer;
}

void VertexArray::bind() const { glBindVertexArray(_rendererId); }
void VertexArray::unbind() const { glBindVertexArray(0); }

void VertexArray::addVertexBuffer(VertexBuffer* vertexBuffer) {
	CAPP_ASSERT(vertexBuffer->getLayout().size(), "Vertex buffer has no layout!");
	_vertexBuffers.push_back(vertexBuffer);

	glBindVertexArray(_rendererId);
	vertexBuffer->bind();

	unsigned i = 0;
	for(const auto& elem : vertexBuffer->getLayout()) {
		glEnableVertexAttribArray(i);

		glVertexAttribPointer(i, elem.getComponentCount(),
		                      SDTypeToOpenGLType(elem.type),
		                      elem.normalized ? GL_TRUE : GL_FALSE,
		                      vertexBuffer->getLayout().getStride(),
		                      reinterpret_cast<const void*>(static_cast<__int64>(elem.offset)));

		++i;
	}

}

void VertexArray::setIndexBuffer(IndexBuffer* indexBuffer) {
	_indexBuffer = indexBuffer;
	
	glBindVertexArray(_rendererId);
	indexBuffer->bind();
}

const std::vector<VertexBuffer*> VertexArray::getVertexBuffers() const { return _vertexBuffers; }
const IndexBuffer* VertexArray::getIndexBuffer() const { return _indexBuffer; }
