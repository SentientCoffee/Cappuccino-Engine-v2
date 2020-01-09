#include "CappPCH.h"
#include "Cappuccino/Rendering/Buffers.h"

#include <glad/glad.h>

using namespace Capp;

// -------------------------------------------------------------
// ----- Vertex buffers ----------------------------------------
// -------------------------------------------------------------

VertexBuffer::VertexBuffer(const std::vector<float>& vertices) :
	_vertexCount(static_cast<unsigned>(vertices.size())), _layout({}) {
	glCreateBuffers(1, &_rendererId);
	glBindBuffer(GL_ARRAY_BUFFER, _rendererId);
	glBufferData(GL_ARRAY_BUFFER, static_cast<long long>(vertices.size() * sizeof(float)), vertices.data(), GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &_rendererId);
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, _rendererId); }
void VertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

unsigned VertexBuffer::getCount() const { return _vertexCount; }

void VertexBuffer::setLayout(const BufferLayout& layout) { _layout = layout; }
const BufferLayout& VertexBuffer::getLayout() const { return _layout; }

// -------------------------------------------------------------
// ----- Index buffers -----------------------------------------
// -------------------------------------------------------------

IndexBuffer::IndexBuffer(const std::vector<unsigned>& indices) :
	_indexCount(static_cast<unsigned>(indices.size())) {
	glCreateBuffers(1, &_rendererId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexCount * sizeof(unsigned), indices.data(), GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &_rendererId);
}

void IndexBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _rendererId); }
void IndexBuffer::unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

unsigned IndexBuffer::getCount() const { return _indexCount; }
