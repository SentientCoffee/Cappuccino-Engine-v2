#include "CappPCH.h"
#include "Cappuccino/Rendering/Buffers.h"

#include <glad/glad.h>

using namespace Capp;

// -------------------------------------------------------------
// ----- Vertex buffers ----------------------------------------
// -------------------------------------------------------------

VertexBuffer::VertexBuffer(const std::vector<float>& vertices, const BufferUsage usage) :
	_vertexCount(static_cast<unsigned>(vertices.size())), _usage(usage), _layout({}) {
	glCreateBuffers(1, &_id);
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<long long>(vertices.size() * sizeof(float)), vertices.data(), static_cast<GLenum>(usage));
}

VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices, const BufferUsage usage) :
	_vertexCount(static_cast<unsigned>(vertices.size())), _usage(usage), _layout({}) {
	glCreateBuffers(1, &_id);
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<long long>(vertices.size() * sizeof(Vertex)), vertices.data(), static_cast<GLenum>(usage));
}

VertexBuffer::VertexBuffer(float* vertices, const unsigned sizeInBytes, const BufferUsage usage) :
	_vertexCount(sizeInBytes / sizeof vertices), _usage(usage), _layout({}) {
	glCreateBuffers(1, &_id);
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<long long>(sizeInBytes), vertices, static_cast<GLenum>(usage));
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &_id);
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, _id); }
void VertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

unsigned VertexBuffer::getCount() const { return _vertexCount; }

void VertexBuffer::setLayout(const BufferLayout& layout) { _layout = layout; }
const BufferLayout& VertexBuffer::getLayout() const { return _layout; }

void VertexBuffer::setBufferData(const std::vector<float>& vertices) const {
	CAPP_ASSERT(_usage == BufferUsage::DynamicDraw, "Buffer must be set as dynamic draw before changing buffer data!");
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<long long>(vertices.size() * sizeof(float)), vertices.data());
}

void VertexBuffer::setBufferData(const std::vector<Vertex>& vertices) const {
	CAPP_ASSERT(_usage == BufferUsage::DynamicDraw, "Buffer must be set as dynamic draw before changing buffer data!");
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<long long>(vertices.size() * sizeof(Vertex)), vertices.data());
}

void VertexBuffer::setBufferData(float* vertices, const unsigned sizeInBytes) const {
	CAPP_ASSERT(_usage == BufferUsage::DynamicDraw, "Buffer must be set as dynamic draw before changing buffer data!");
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<long long>(sizeInBytes), vertices);
}

// -------------------------------------------------------------
// ----- Index buffers -----------------------------------------
// -------------------------------------------------------------

IndexBuffer::IndexBuffer(const std::vector<unsigned>& indices) :
	_indexCount(static_cast<unsigned>(indices.size())) {
	glCreateBuffers(1, &_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &_id);
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id); }
void IndexBuffer::unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

unsigned IndexBuffer::getCount() const { return _indexCount; }
