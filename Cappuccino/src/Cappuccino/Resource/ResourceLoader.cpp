#include "CappPCH.h"
#include "Cappuccino/Resource/ResourceLoader.h"

using namespace Capp;

// ------------------------------------------------------------------------------------------
// ----- CUBE Loader ------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------

Texture3D* ResourceLoader::loadCUBE(const std::string& filepath) {
	std::ifstream lutFile(filepath);
	CAPP_ASSERT(lutFile.good(), "Could not open file!\n\tLUT filepath: {0}", filepath);

	std::vector<glm::vec3> lutData;
	lutData.reserve(static_cast<unsigned>(pow(64, 3)));
	unsigned lutSize = 0;
	
	while(!lutFile.eof()) {
		std::string lutLine;
		getline(lutFile, lutLine);

		if(lutLine.empty() || lutLine[0] == '#') {
			continue;
		}

		if(lutLine.compare(0, 11, "LUT_3D_SIZE") == 0) {
			std::string token;
			std::istringstream sin(lutLine);

			sin >> token >> lutSize;
			continue;
		}

		if(lutLine.compare(0, 5, "TITLE") == 0) {
			std::string token, title;
			std::istringstream sin(lutLine);

			sin >> token >> title;
			continue;
		}
		
		glm::vec3 line;
		const bool lutReadStatus = sscanf(lutLine.c_str(), "%f %f %f", &line.r, &line.g, &line.b);
		CAPP_ASSERT(lutReadStatus, "Failed to read colour values!\n\tLUT file: {0}", filepath);
		lutData.push_back(line);
	}
	
	CAPP_ASSERT(lutData.size() == pow(lutSize, 3), "LUT size is incorrect.\n\tGiven LUT size: {0}\n\tExpected LUT size: {1}", lutData.size(), pow(lutSize, 3));
	
	return new Texture3D(lutSize, lutSize, lutSize, lutData.data());
}

// ------------------------------------------------------------------------------------------
// ----- OBJ Loader -------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------

struct TriangleFace {
	std::vector<unsigned> vertIndices = { 0, 0, 0 };
	std::vector<unsigned> normIndices = { 0, 0, 0 };
	std::vector<unsigned> uvIndices = { 0, 0, 0 };
};

std::tuple<VertexBuffer*, IndexBuffer*> ResourceLoader::loadOBJ(const std::string& filepath) {
	std::ifstream file(filepath.data());
	CAPP_ASSERT(file.good(), "Could not open file!\n\tOBJ filepath: {0}", filepath);

	const int lineBufferSize = 256;
	char lineContent[lineBufferSize];

	std::vector<glm::vec3> positions; positions.reserve(1000000);
	std::vector<glm::vec2> uvCoords; uvCoords.reserve(1000000);
	std::vector<glm::vec3> normals; normals.reserve(1000000);
	std::vector<TriangleFace> faces; faces.reserve(1000000);

	while(!file.eof()) {
		file.getline(lineContent, lineBufferSize);

		if(lineContent[0] == 'v') {
			switch(lineContent[1]) {
				case ' ':
				{
					glm::vec3 temp;
					const bool vertexReadStatus = sscanf(lineContent, "v %f %f %f", &temp.x, &temp.y, &temp.z);
					CAPP_ASSERT(vertexReadStatus, "Failed to read vertex data of OBJ file: {0}", filepath);
					positions.push_back(temp);
					break;
				}

				case 't':
				{
					glm::vec2 temp;
					const bool uvReadStatus = sscanf(lineContent, "vt %f %f", &temp.x, &temp.y);
					CAPP_ASSERT(uvReadStatus, "Failed to read texture data of OBJ file: {0}",  filepath);
					uvCoords.push_back(temp);
					break;
				}

				case 'n':
				{
					glm::vec3 temp;
					const bool normalReadStatus = sscanf(lineContent, "vn %f %f %f", &temp.x, &temp.y, &temp.z);
					CAPP_ASSERT(normalReadStatus, "Failed to read normal data of mesh OBJ file: {0}", filepath);
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

			CAPP_ASSERT(indexReadStatus, "Failed to read index data of mesh OBJ file: {0}", filepath);
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
