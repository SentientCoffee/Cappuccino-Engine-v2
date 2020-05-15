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

Mesh::Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices) :
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
