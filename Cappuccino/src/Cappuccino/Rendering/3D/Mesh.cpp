#include "CappPCH.h"
#include "Cappuccino/Rendering/Mesh.h"
#include "Cappuccino/Resource/AssetLoader.h"

using namespace Capp;

Mesh::Mesh(const std::string& name, const std::string& filepath) :
	_name(name)
{
	_vao = VertexArray::create();
	AssetLoader::MeshData data = AssetLoader::loadOBJFile(filepath);

	const BufferLayout layout = {
		{ ShaderDataType::Vec3, "inPosition" },
		{ ShaderDataType::Vec2, "inUV" },
		{ ShaderDataType::Vec3, "inNormal" },
	};

	data.vbo->setLayout(layout);
	_vao->addVertexBuffer(data.vbo);
	_vao->setIndexBuffer(data.ibo);
}

Mesh::Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) :
	_name(name)
{
	_vao = VertexArray::create();
	const auto vbo = VertexBuffer::create(vertices);
	const auto ibo = IndexBuffer::create(indices);

	const BufferLayout layout = {
		{ ShaderDataType::Vec3, "inPosition" },
		{ ShaderDataType::Vec2, "inUV" },
		{ ShaderDataType::Vec3, "inNormal" },
	};

	vbo->setLayout(layout);
	_vao->addVertexBuffer(vbo);
	_vao->setIndexBuffer(ibo);
}

Mesh::Mesh(const std::string& name, Vertex* vertices, const uint32_t vertexCount, uint32_t* indices, const uint32_t indexCount) :
	_name(name)
{
	_vao = VertexArray::create();
	const auto vbo = VertexBuffer::create(vertices, vertexCount * sizeof(Vertex));
	const auto ibo = IndexBuffer::create(indices, indexCount);

	const BufferLayout layout = {
		{ ShaderDataType::Vec3, "inPosition" },
		{ ShaderDataType::Vec2, "inUV" },
		{ ShaderDataType::Vec3, "inNormal" },
	};

	vbo->setLayout(layout);
	_vao->addVertexBuffer(vbo);
	_vao->setIndexBuffer(ibo);
}