#pragma once

#include "Cappuccino/Rendering/BufferLayout.h"

#include <glm/glm.hpp>

namespace Capp {

	struct Vertex {
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
		glm::vec2 uvCoords = { 0.0f, 0.0f };
		glm::vec3 normal = { 0.0f, 0.0f, 0.0f };

		bool operator<(const Vertex other) const {
			return memcmp(static_cast<void const*>(this), static_cast<void const*>(&other), sizeof(Vertex)) > 0;
		};
	};

	class VertexBuffer {
	public:

		VertexBuffer() = default;
		VertexBuffer(const std::vector<float>& vertices);
		VertexBuffer(const std::vector<Vertex>& vertices);
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
