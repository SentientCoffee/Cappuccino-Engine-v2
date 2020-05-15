#include "CappPCH.h"
#include "Cappuccino/Rendering/VertexArray.h"

#include <glad/glad.h>

using namespace Capp;

static GLenum SDTypeToOpenGLType(const ShaderDataType type) {
	switch(type) {
		case ShaderDataType::Bool:		return GL_BOOL;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Float:

		case ShaderDataType::Vec2:
		case ShaderDataType::Vec3:
		case ShaderDataType::Vec4:

		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:		return GL_FLOAT;
	}

	CAPP_ASSERT(false, "Unknown shader data type!");
	return 0;
}

VertexArray::VertexArray() :
	_indexBuffer(nullptr)
{
	glCreateVertexArrays(1, &_id);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &_id);
	_vertexBuffers.clear();
}

void VertexArray::bind() const { glBindVertexArray(_id); }
void VertexArray::unbind() { glBindVertexArray(0); }

void VertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
	CAPP_ASSERT(vertexBuffer->getLayout().size() != 0, "Vertex buffer has no layout!");
	_vertexBuffers.push_back(vertexBuffer);

	glBindVertexArray(_id);
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
	glBindVertexArray(0);
}

void VertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
	_indexBuffer = indexBuffer;
	
	glBindVertexArray(_id);
	indexBuffer->bind();
	glBindVertexArray(0);
}
