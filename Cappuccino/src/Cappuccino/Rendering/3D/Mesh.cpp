#include "CappPCH.h"
#include "Cappuccino/Rendering/3D/Mesh.h"

using namespace Capp;

struct TriangleFace {
	std::vector<unsigned> vertIndices { 0, 0, 0 };
	std::vector<unsigned> normIndices { 0, 0, 0 };
	std::vector<unsigned> uvIndices { 0, 0, 0 };
};

Mesh::Mesh(const std::string& filepath) :
	_meshPath(filepath) {
	_vao = new VertexArray;
	auto [vbo, ibo] = loadMesh(filepath);

	const BufferLayout layout = {
		{ ShaderDataType::Vec3, "inPosition" },
		{ ShaderDataType::Vec2, "inUV" },
		{ ShaderDataType::Vec3, "inNormal" },
	};

	vbo->setLayout(layout);	
	_vao->addVertexBuffer(vbo);
	_vao->setIndexBuffer(ibo);
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned>& indices) {
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

std::tuple<VertexBuffer*, IndexBuffer*> Mesh::loadMesh(const std::string& filepath) {
	std::ifstream file(filepath.data());

	if(!file.good()) {
		CAPP_PRINT_ERROR("File: {0}", filepath);
		CAPP_ASSERT(file.good(), "Could not open file!");
	}

	const int lineBufferSize = 256;
	char lineContent[lineBufferSize];
	
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uvCoords;
	std::vector<glm::vec3> normals;
	std::vector<TriangleFace> faces;

	while(!file.eof()) {
		file.getline(lineContent, lineBufferSize);

		if(lineContent[0] == 'v') {
			switch(lineContent[1]) {
				case ' ': {
					glm::vec3 temp;
					const bool vertexReadStatus = sscanf(lineContent, "v %f %f %f", &temp.x, &temp.y, &temp.z);
					if(!vertexReadStatus) {
						CAPP_PRINT_ERROR("File: {0}", filepath);
						CAPP_ASSERT(vertexReadStatus, "Failed to read vertex data!");
					}
					positions.push_back(temp);
					break;
				}

				case 't': {
					glm::vec2 temp;
					const bool uvReadStatus = sscanf(lineContent, "vt %f %f", &temp.x, &temp.y);
					if(!uvReadStatus) {
						CAPP_PRINT_ERROR("File: {0}", filepath);
						CAPP_ASSERT(uvReadStatus, "Failed to read texture data!");
					}
					uvCoords.push_back(temp);
					break;
				}

				case 'n': {
					glm::vec3 temp;
					const bool normalReadStatus = sscanf(lineContent, "vn %f %f %f", &temp.x, &temp.y, &temp.z);
					if(!normalReadStatus) {
						CAPP_PRINT_ERROR("File: {0}", filepath);
						CAPP_ASSERT(normalReadStatus, "Failed to read normal data!");
					}
					normals.push_back(temp);
					break;
				}
				default: break;
			}
		}
		else if(lineContent[0] == 'f') {
			TriangleFace temp;
			const bool indexReadStatus = sscanf(lineContent, "f %d/%d/%d %d/%d/%d %d/%d/%d",
			                                    &temp.vertIndices[0], &temp.uvIndices[0], &temp.normIndices[0],
			                                    &temp.vertIndices[1], &temp.uvIndices[1], &temp.normIndices[1],
			                                    &temp.vertIndices[2], &temp.uvIndices[2], &temp.normIndices[2]);
			
			if(!indexReadStatus) {
				CAPP_PRINT_ERROR("File: {0}", filepath);
				CAPP_ASSERT(indexReadStatus, "Failed to read normal data!");
			}
			faces.push_back(temp);
		}
	}
	
	file.close();

	std::vector<Vertex> allVertices;
	for(auto& face : faces) {
		for(unsigned i = 0; i < face.vertIndices.size(); ++i) {
			unsigned vertIndex = face.vertIndices[i];
			unsigned uvIndex = face.uvIndices[i];
			unsigned normIndex = face.normIndices[i];

			Vertex vertex = {
				positions[vertIndex - 1],
				uvCoords[uvIndex - 1],
				normals[normIndex - 1]
			};

			allVertices.push_back(vertex);
		}
	}

	std::vector<unsigned> indices;
	std::vector<Vertex> vertices;
	std::map<Vertex, unsigned> vertexIndexMap;
	for(auto& vertex : allVertices) {
		
		unsigned index;

		auto it = vertexIndexMap.find(vertex);
		if(it != vertexIndexMap.end()) {
			// Old vertex, reuse the already generated index
			index = it->second;
		}
		else {
			// New vertex, add to final vertices array and generate new index
			vertices.push_back(vertex);
			index = static_cast<unsigned>(vertices.size() - 1);
		}
		
		indices.push_back(index);
		vertexIndexMap[vertex] = index;
	}
	
	return { new VertexBuffer(vertices), new IndexBuffer(indices) };
}
