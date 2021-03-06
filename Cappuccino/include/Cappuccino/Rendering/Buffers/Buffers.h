#pragma once

#include "Cappuccino/Core/Memory.h"
#include "Cappuccino/Rendering/Buffers/BufferLayout.h"

#include <glm/glm.hpp>

namespace Capp {

	enum class BufferUsage : uint32_t {
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
		VertexBuffer(const float* vertices, uint32_t sizeInBytes, BufferUsage usage = BufferUsage::StaticDraw);
		VertexBuffer(const Vertex* vertices, uint32_t sizeInBytes, BufferUsage usage = BufferUsage::StaticDraw);

		template<size_t Size> VertexBuffer(const std::array<float, Size>& vertices, const BufferUsage usage = BufferUsage::StaticDraw) :
			VertexBuffer(vertices.data(), Size * sizeof(float), usage) {}
		template<size_t Size> VertexBuffer(const std::array<Vertex, Size>& vertices, const BufferUsage usage = BufferUsage::StaticDraw) :
			VertexBuffer(vertices.data(), Size * sizeof(Vertex), usage) {}

		~VertexBuffer();

		static Ref<VertexBuffer> create(const std::vector<float>& vertices, BufferUsage usage = BufferUsage::StaticDraw) {
			return Memory::createRef<VertexBuffer>(vertices, usage);
		}
		static Ref<VertexBuffer> create(const std::vector<Vertex>& vertices, BufferUsage usage = BufferUsage::StaticDraw) {
			return Memory::createRef<VertexBuffer>(vertices, usage);
		}
		template<size_t Size>
		static Ref<VertexBuffer> create(const std::array<float, Size>& vertices, BufferUsage usage = BufferUsage::StaticDraw) {
			return Memory::createRef<VertexBuffer>(vertices, usage);
		}
		template<size_t Size>
		static Ref<VertexBuffer> create(const std::array<Vertex, Size>& vertices, BufferUsage usage = BufferUsage::StaticDraw) {
			return Memory::createRef<VertexBuffer>(vertices, usage);
		}
		static Ref<VertexBuffer> create(const float* vertices, uint32_t sizeInBytes, BufferUsage usage = BufferUsage::StaticDraw) {
			return Memory::createRef<VertexBuffer>(vertices, sizeInBytes, usage);
		}
		static Ref<VertexBuffer> create(const Vertex* vertices, uint32_t sizeInBytes, BufferUsage usage = BufferUsage::StaticDraw) {
			return Memory::createRef<VertexBuffer>(vertices, sizeInBytes, usage);
		}

		void bind() const;
		static void unbind();

		uint32_t getCount() const { return _vertexCount; }

		void setLayout(const BufferLayout& layout) { _layout = layout; }
		const BufferLayout& getLayout() const { return _layout; }

		void setBufferData(const std::vector<float>& vertices) const;
		void setBufferData(const std::vector<Vertex>& vertices) const;
		void setBufferData(const float* vertices, uint32_t sizeInBytes) const;
		void setBufferData(const Vertex* vertices, uint32_t sizeInBytes) const;

		template<size_t Size>
		void setBufferData(const std::array<float, Size>& vertices) const {
			setBufferData(vertices.data(), static_cast<uint32_t>(Size * sizeof(float)));
		}
		template<size_t Size>
		void setBufferData(const std::array<Vertex, Size>& vertices) const {
			setBufferData(vertices.data(), static_cast<uint32_t>(Size * sizeof(Vertex)));
		}

	private:

		uint32_t _id = 0;
		mutable uint32_t _vertexCount = 0;
		BufferUsage _usage = BufferUsage::StaticDraw;
		BufferLayout _layout;

	};

	class IndexBuffer {
	public:

		IndexBuffer() = default;
		IndexBuffer(const uint32_t* indices, uint32_t count, BufferUsage usage = BufferUsage::StaticDraw);
		IndexBuffer(const std::vector<uint32_t>& indices, BufferUsage usage = BufferUsage::StaticDraw);
		template<size_t Size> IndexBuffer(const std::array<uint32_t, Size>& indices, const BufferUsage usage = BufferUsage::StaticDraw) :
			IndexBuffer(indices.data(), Size, usage) {}
		
		~IndexBuffer();

		static Ref<IndexBuffer> create(const std::vector<uint32_t>& indices, BufferUsage usage = BufferUsage::StaticDraw) {
			return Memory::createRef<IndexBuffer>(indices, usage);
		}
		template<size_t Size>
		static Ref<IndexBuffer> create(const std::array<uint32_t, Size>& indices, BufferUsage usage = BufferUsage::StaticDraw) {
			return Memory::createRef<IndexBuffer>(indices, usage);
		}
		static Ref<IndexBuffer> create(const uint32_t* indices, uint32_t count, BufferUsage usage = BufferUsage::StaticDraw) {
			return Memory::createRef<IndexBuffer>(indices, count, usage);
		}

		void bind() const;
		static void unbind();

		uint32_t getCount() const { return _indexCount; }

		void setBufferData(const std::vector<uint32_t>& indices) const;
		void setBufferData(const uint32_t* indices, uint32_t count) const;

		template<size_t Size> void setBufferData(const std::array<uint32_t, Size>& indices) const {
			setBufferData(indices.data(), Size);
		}
		
	private:

		uint32_t _id = 0;
		mutable uint32_t _indexCount = 0;
		BufferUsage _usage = BufferUsage::StaticDraw;

	};
}
