#include "CappPCH.h"
#include "Cappuccino/Rendering/Buffers/Buffers.h"

#include <glad/glad.h>

using namespace Capp;

// -------------------------------------------------------------
// ----- Vertex buffers ----------------------------------------
// -------------------------------------------------------------

VertexBuffer::VertexBuffer(const std::vector<float>& vertices, const BufferUsage usage) :
	_vertexCount(static_cast<uint32_t>(vertices.size())), _usage(usage), _layout({})
{
	glCreateBuffers(1, &_id);
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data(), static_cast<GLenum>(usage));
}

VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices, const BufferUsage usage) :
	_vertexCount(static_cast<uint32_t>(vertices.size())), _usage(usage), _layout({})
{
	glCreateBuffers(1, &_id);
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data(), static_cast<GLenum>(usage));
}

VertexBuffer::VertexBuffer(const float* vertices, const uint32_t sizeInBytes, const BufferUsage usage) :
	_vertexCount(sizeInBytes / sizeof(float)), _usage(usage), _layout({})
{
	glCreateBuffers(1, &_id);
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeInBytes), vertices, static_cast<GLenum>(usage));
}

VertexBuffer::VertexBuffer(const Vertex* vertices, const uint32_t sizeInBytes, const BufferUsage usage) :
	_vertexCount(sizeInBytes / sizeof(Vertex)), _usage(usage), _layout({})
{
	glCreateBuffers(1, &_id);
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeInBytes), vertices, static_cast<GLenum>(usage));
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &_id);
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, _id); }
void VertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::setBufferData(const std::vector<float>& vertices) const {
	CAPP_ASSERT(_usage == BufferUsage::DynamicDraw, "Buffer must be set as dynamic draw before changing buffer data!");
	glNamedBufferSubData(_id, 0, static_cast<GLsizeiptr>(vertices.size() * sizeof(float)), vertices.data());
	_vertexCount = static_cast<uint32_t>(vertices.size());
}

void VertexBuffer::setBufferData(const std::vector<Vertex>& vertices) const {
	CAPP_ASSERT(_usage == BufferUsage::DynamicDraw, "Buffer must be set as dynamic draw before changing buffer data!");
	glNamedBufferSubData(_id, 0, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), vertices.data());
	_vertexCount = static_cast<uint32_t>(vertices.size());
}

void VertexBuffer::setBufferData(const float* vertices, const uint32_t sizeInBytes) const {
	CAPP_ASSERT(_usage == BufferUsage::DynamicDraw, "Buffer must be set as dynamic draw before changing buffer data!");
	glNamedBufferSubData(_id, 0, static_cast<GLsizeiptr>(sizeInBytes), vertices);
	_vertexCount = static_cast<uint32_t>(sizeInBytes / sizeof(float));
}

void VertexBuffer::setBufferData(const Vertex* vertices, const uint32_t sizeInBytes) const {
	CAPP_ASSERT(_usage == BufferUsage::DynamicDraw, "Buffer must be set as dynamic draw before changing buffer data!");
	glNamedBufferSubData(_id, 0, static_cast<GLsizeiptr>(sizeInBytes), vertices);
	_vertexCount = static_cast<uint32_t>(sizeInBytes / sizeof(Vertex));
}

// -------------------------------------------------------------
// ----- Index buffers -----------------------------------------
// -------------------------------------------------------------

IndexBuffer::IndexBuffer(const std::vector<uint32_t>& indices, BufferUsage usage) :
	_indexCount(static_cast<uint32_t>(indices.size())), _usage(usage) {
	glCreateBuffers(1, &_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(uint32_t)), indices.data(), static_cast<GLenum>(usage));
}

IndexBuffer::IndexBuffer(const uint32_t* indices, const uint32_t count, const BufferUsage usage) :
	_indexCount(count), _usage(usage) {
	glCreateBuffers(1, &_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, static_cast<GLenum>(usage));
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &_id);
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id); }
void IndexBuffer::unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

void IndexBuffer::setBufferData(const std::vector<uint32_t>& indices) const {
	CAPP_ASSERT(_usage == BufferUsage::DynamicDraw, "Buffer must be set as dynamic draw before changing buffer data!");
	glNamedBufferSubData(_id, 0, static_cast<GLsizeiptr>(indices.size() * sizeof(uint32_t)), indices.data());
	_indexCount = static_cast<uint32_t>(indices.size());
}

void IndexBuffer::setBufferData(const uint32_t* indices, const uint32_t count) const {
	CAPP_ASSERT(_usage == BufferUsage::DynamicDraw, "Buffer must be set as dynamic draw before changing buffer data!");
	glNamedBufferSubData(_id, 0, static_cast<GLsizeiptr>(count * sizeof(uint32_t)), indices);
	_indexCount = count;
}