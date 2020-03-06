#include "CappPCH.h"
#include "Cappuccino/Rendering/Mesh.h"
#include "Cappuccino/Resource/ResourceLoader.h"

using namespace Capp;

Mesh::Mesh(const std::string& name, const std::string& filepath) :
	_name(name), _meshPath(filepath) {
	_vao = new VertexArray;
	auto [vbo, ibo] = ResourceLoader::loadOBJFile(filepath);

	const BufferLayout layout = {
		{ ShaderDataType::Vec3, "inPosition" },
		{ ShaderDataType::Vec2, "inUV" },
		{ ShaderDataType::Vec3, "inNormal" },
	};

	vbo->setLayout(layout);
	_vao->addVertexBuffer(vbo);
	_vao->setIndexBuffer(ibo);
}

Mesh::Mesh(const std::string& name, const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices) :
	_name(name) {
	_vao = new VertexArray;
	const auto vbo = new VertexBuffer(vertices);
	const auto ibo = new IndexBuffer(indices);

	const BufferLayout layout = {
		{ ShaderDataType::Vec3, "inPosition" },
		{ ShaderDataType::Vec2, "inUV" },
		{ ShaderDataType::Vec3, "inNormal" },
	};

	vbo->setLayout(layout);
	_vao->addVertexBuffer(vbo);
	_vao->setIndexBuffer(ibo);
}

Mesh::~Mesh() {
	delete _vao;
}

VertexArray* Mesh::getVAO() const { return _vao; }