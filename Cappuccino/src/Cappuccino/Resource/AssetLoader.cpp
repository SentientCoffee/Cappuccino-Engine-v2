#include "CappPCH.h"
#include "Cappuccino/Resource/AssetLoader.h"

#include <stb/stb_image.h>

using namespace Capp;

// ------------------------------------------------------------------------------------------
// ----- CUBE Loader ------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------

AssetLoader::LUTData AssetLoader::loadCUBEFile(const std::string& filepath) {
	std::ifstream lutFile(filepath);
	CAPP_ASSERT(lutFile.good(), "Failed to read LUT file!\n\tFilepath: {0}", filepath);

	std::vector<glm::vec3> lutData;
	uint32_t lutSize = 0;
	
	while(!lutFile.eof()) {
		std::string lutLine;
		getline(lutFile, lutLine);

		if(lutLine.empty() || lutLine[0] == '#') {
			continue;
		}

		if(lutLine.compare(0, 5, "TITLE") == 0) {
			continue;
		}
		
		if(lutLine.compare(0, 11, "LUT_3D_SIZE") == 0) {
			std::string token;
			std::istringstream sin(lutLine);

			sin >> token >> lutSize;
			lutData.reserve(static_cast<uint32_t>(pow(lutSize, 3)));
			continue;
		}

		glm::vec3 line;
		const bool lutReadStatus = sscanf(lutLine.c_str(), "%f %f %f", &line.r, &line.g, &line.b);
		CAPP_ASSERT(lutReadStatus, "Failed to read LUT colour values!\n\tFilepath: {0}", filepath);
		lutData.push_back(line);
	}
	
	lutFile.close();

	CAPP_ASSERT(lutData.size() == pow(lutSize, 3), "LUT size is incorrect!\n\tGiven LUT size: {0}\n\tExpected LUT size: {1}\n\tFilepath: {2}", lutData.size(), pow(lutSize, 3), filepath);
	
	return { lutSize, std::move(lutData) };
}

// ------------------------------------------------------------------------------------------
// ----- Image File Loader ------------------------------------------------------------------
// ------------------------------------------------------------------------------------------

AssetLoader::ImageData AssetLoader::loadImageFile(const std::string& filepath, const bool flipped, const int desiredChannels) {
	stbi_set_flip_vertically_on_load(flipped);
	int width, height, channels;
	stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, desiredChannels);

	CAPP_ASSERT(data != nullptr && width > 0 && height > 0 && channels > 0, "Failed to load image file!\n\tFilepath: {0}", filepath);

	return { data, static_cast<uint32_t>(width), static_cast<uint32_t>(height), static_cast<uint32_t>(channels) };
}


// ------------------------------------------------------------------------------------------
// ----- OBJ Loader -------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------

struct TriangleFace {
	std::array<uint32_t, 3> vertIndices = { 0, 0, 0 };
	std::array<uint32_t, 3> normIndices = { 0, 0, 0 };
	std::array<uint32_t, 3> uvIndices   = { 0, 0, 0 };
};

AssetLoader::MeshData AssetLoader::loadOBJFile(const std::string& filepath) {
	std::ifstream objFile(filepath.data());
	CAPP_ASSERT(objFile.good(), "Failed to read OBJ file!\n\tFilepath: {0}", filepath);

	const int lineBufferSize = 256;
	char lineContent[lineBufferSize];

	std::vector<glm::vec3> positions; positions.reserve(1000000);
	std::vector<glm::vec2> uvCoords; uvCoords.reserve(1000000);
	std::vector<glm::vec3> normals; normals.reserve(1000000);
	std::vector<TriangleFace> faces; faces.reserve(1000000);

	while(!objFile.eof()) {
		objFile.getline(lineContent, lineBufferSize);

		if(lineContent[0] == 'v') {
			switch(lineContent[1]) {
				case ' ':
				{
					glm::vec3 temp;
					const bool vertexReadStatus = sscanf(lineContent, "v %f %f %f", &temp.x, &temp.y, &temp.z);
					CAPP_ASSERT(vertexReadStatus, "Failed to read vertex data!\n\tFilepath: {0}", filepath);
					positions.push_back(temp);
					break;
				}

				case 't':
				{
					glm::vec2 temp;
					const bool uvReadStatus = sscanf(lineContent, "vt %f %f", &temp.x, &temp.y);
					CAPP_ASSERT(uvReadStatus, "Failed to read UV data!\n\tFilepath: {0}",  filepath);
					uvCoords.push_back(temp);
					break;
				}

				case 'n':
				{
					glm::vec3 temp;
					const bool normalReadStatus = sscanf(lineContent, "vn %f %f %f", &temp.x, &temp.y, &temp.z);
					CAPP_ASSERT(normalReadStatus, "Failed to read normal data!\n\tFilepath: {0}", filepath);
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

			CAPP_ASSERT(indexReadStatus, "Failed to read index data!\n\tFilepath: {0}", filepath);
			faces.push_back(temp);
		}
	}

	objFile.close();

	std::vector<Vertex> allVertices; allVertices.reserve(1000000);
	for(auto& face : faces) {
		for(uint32_t i = 0; i < face.vertIndices.size(); ++i) {
			uint32_t vertIndex = face.vertIndices[i];
			uint32_t uvIndex = face.uvIndices[i];
			uint32_t normIndex = face.normIndices[i];

			Vertex vertex = {
				positions[vertIndex - 1],
				uvCoords[uvIndex - 1],
				normals[normIndex - 1]
			};

			allVertices.push_back(vertex);
		}
	}

	std::vector<uint32_t> indices; indices.reserve(1000000);
	std::vector<Vertex> vertices; vertices.reserve(1000000);
	std::map<Vertex, uint32_t> vertexIndexMap;

	for(auto& vertex : allVertices) {

		uint32_t index;

		auto it = vertexIndexMap.find(vertex);
		if(it != vertexIndexMap.end()) {
			// Old vertex, reuse the already generated index
			index = it->second;
		}
		else {
			// New vertex, add to final vertices array and generate new index
			vertices.push_back(vertex);
			index = static_cast<uint32_t>(vertices.size() - 1);
		}

		indices.push_back(index);
		vertexIndexMap[vertex] = index;
	}

	return { VertexBuffer::create(vertices), IndexBuffer::create(indices) };
}

// ------------------------------------------------------------------------------------------
// ----- Text File Loader -------------------------------------------------------------------
// ------------------------------------------------------------------------------------------

std::string AssetLoader::readTextFile(const std::string& filepath) {
	std::ifstream txtFile(filepath.data());
	std::stringstream fileContent;
	CAPP_ASSERT(fileContent.good(), "Failed to read text file!\n\tFilepath: {0}", filepath);

	fileContent << txtFile.rdbuf();
	txtFile.close();

	return fileContent.str();
}