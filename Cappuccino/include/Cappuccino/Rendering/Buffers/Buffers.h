#pragma once

#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Rendering/Buffers/BufferLayout.h"

#include <glm/glm.hpp>

namespace Capp {

	enum class BufferUsage : unsigned int {
		// From glad.h
		StreamDraw  = 0x88E0,	// GL_STREAM_DRAW
		StreamRead  = 0x88E1,	// GL_STREAM_READ
		StreamCopy  = 0x88E2,	// GL_STREAM_COPY
		
		StaticDraw  = 0x88E4,	// GL_STATIC_DRAW
		StaticRead  = 0x88E5,	// GL_STATIC_READ
		StaticCopy  = 0x88E6,	// GL_STATIC_COPY
		
		DynamicDraw = 0x88E8,	// GL_DYNAMIC_DRAW
		DynamicRead = 0x88E9,	// GL_DYNAMIC_READ
		DynamicCopy = 0x88EA,	// GL_DYNAMIC_COPY
	};
	
	struct Vertex {
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec2 uvCoords = { 0.0f, 0.0f };
		glm::vec3 normal   = { 0.0f, 0.0f, 0.0f };

		bool operator<(const Vertex other) const {
			return memcmp(static_cast<void const*>(this), static_cast<void const*>(&other), sizeof(Vertex)) > 0;
		}
	};
	
	class VertexBuffer {
	public:

		VertexBuffer() = default;
		VertexBuffer(const std::vector<float>& vertices, BufferUsage usage = BufferUsage::StaticDraw);
		VertexBuffer(const std::vector<Vertex>& vertices, BufferUsage usage = BufferUsage::StaticDraw);
		VertexBuffer(float* vertices, unsigned sizeInBytes, BufferUsage usage = BufferUsage::StaticDraw);
		~VertexBuffer();

		static Ref<VertexBuffer> create(const std::vector<float>& vertices, BufferUsage usage = BufferUsage::StaticDraw) {
			return Memory::createRef<VertexBuffer>(vertices, usage);
		}
		static Ref<VertexBuffer> create(const std::vector<Vertex>& vertices, BufferUsage usage = BufferUsage::StaticDraw) {
			return Memory::createRef<VertexBuffer>(vertices, usage);
		}
		static Ref<VertexBuffer> create(float* vertices, unsigned sizeInBytes, BufferUsage usage = BufferUsage::StaticDraw) {
			return Memory::createRef<VertexBuffer>(vertices, sizeInBytes, usage);
		}

		void bind() const;
		static void unbind();

		unsigned int getCount() const;

		void setLayout(const BufferLayout& layout);
		const BufferLayout& getLayout() const;

		void setBufferData(const std::vector<float>& vertices) const;
		void setBufferData(const std::vector<Vertex>& vertices) const;
		void setBufferData(float* vertices, unsigned sizeInBytes) const;

	private:

		unsigned _id = 0;
		mutable unsigned _vertexCount = 0;
		BufferUsage _usage = BufferUsage::StaticDraw;
		BufferLayout _layout;

	};

	class IndexBuffer {
	public:

		IndexBuffer() = default;
		IndexBuffer(const std::vector<unsigned>& indices, BufferUsage usage = BufferUsage::StaticDraw);
		IndexBuffer(unsigned* indices, unsigned count, BufferUsage usage = BufferUsage::StaticDraw);
		~IndexBuffer();

		static Ref<IndexBuffer> create(const std::vector<unsigned>& indices, BufferUsage usage = BufferUsage::StaticDraw) {
			return Memory::createRef<IndexBuffer>(indices, usage);
		}
		static Ref<IndexBuffer> create(unsigned* indices, unsigned count, BufferUsage usage = BufferUsage::StaticDraw) {
			return Memory::createRef<IndexBuffer>(indices, count, usage);
		}

		void bind() const;
		static void unbind();

		unsigned int getCount() const { return _indexCount; }

		void setBufferData(const std::vector<unsigned>& indices) const;
		void setBufferData(unsigned* indices, unsigned count) const;
		
	private:

		unsigned _id = 0;
		mutable unsigned _indexCount = 0;
		BufferUsage _usage = BufferUsage::StaticDraw;

	};
	
}
