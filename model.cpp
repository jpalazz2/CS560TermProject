#include "model.hpp"

Model::Model(const char *fileName) {
	std::ifstream inFile(fileName);
	if (!inFile.is_open()) {
		std::cerr << "Failed to open file: " << fileName << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::string line;
	// Vectors to store the obj file information
	std::vector<glm::vec3> objVertices;
	std::vector<glm::vec3> faces;
	std::vector<glm::vec3> normals;
	std::map<int, int> vertToNormalMap;
	// Parse the file
	while (std::getline(inFile, line)) {
		std::istringstream ss(line);
		std::string operation;
		ss >> operation;
		if (!operation.compare("v")) {
			float a, b, c;
			if (!(ss >> a >> b >> c)) {
				std::cerr << "Bad object file! " << fileName << std::endl;
				std::exit(EXIT_FAILURE);
			}
			objVertices.push_back(glm::vec3{a, b, c});
		} else if (!operation.compare("f")) {
			std::string a, b, c;
			if (!(ss >> a >> b >> c)) {
				std::cerr << "Bad object file! " << fileName << std::endl;
				std::exit(EXIT_FAILURE);
			}
			auto intify = [&](std::string str) { 
				int vertexNum = std::stoi(str.substr(0, str.find_first_of("/"))) - 1;
				int normalNum = std::stoi(str.substr(str.find_last_of("/") + 1)) - 1;
				auto it = vertToNormalMap.find(vertexNum);
				if (it != vertToNormalMap.end()) {
					assert(normalNum == it->second);
				}
				vertToNormalMap.insert({vertexNum, normalNum});
				return vertexNum;
			};
			faces.push_back(glm::vec3{intify(a), intify(b), intify(c)});
		} else if (!operation.compare("vn")) {
			float a, b, c;
			if (!(ss >> a >> b >> c)) {
				std::cerr << "Bad object file! " << fileName << std::endl;
				std::exit(EXIT_FAILURE);
			}
			normals.push_back(glm::vec3{a, b, c});
		}
	}
	inFile.close();
	assert(faces.size() > 0);
	assert(objVertices.size() > 0);
	assert(normals.size() > 0);
	assert(vertToNormalMap.size() > 0);
	// Need space for three entries for each vertex, plus three for the normals
	vertices = new GLfloat[6 * objVertices.size()];
	// Each face entry requires 9 indices
	// 3 vertices per face, each of which require 3 indices
	indices = new GLuint[3 * faces.size()];
	verticesSize = 6 * objVertices.size();
	indicesSize = 3 * faces.size();
	// Load the data into arrays for glDrawElements();
	for (std::size_t i = 0; i < objVertices.size(); i++) {
		glm::vec3 vec = objVertices[i];
		vertices[6 * i] = vec.x;
		vertices[6 * i + 1] = vec.y;
		vertices[6 * i + 2] = vec.z;
	}
	for (std::size_t i = 0; i < faces.size(); i++) {
		glm::vec3 vec = faces[i];
		indices[i * 3] = vec.x;
		indices[i * 3 + 1] = vec.y;
		indices[i * 3 + 2] = vec.z;
	}
	for (auto it = vertToNormalMap.begin(); it != vertToNormalMap.end(); ++it) {
		int vertexNum = it->first, normalNum = it->second;
		glm::vec3 vec = normals[normalNum];
		vertices[vertexNum * 6 + 3] = vec.x;
		vertices[vertexNum * 6 + 4] = vec.y;
		vertices[vertexNum * 6 + 5] = vec.z;
	}
}

Model::~Model() {
	delete[] vertices;
	delete[] indices;
	vertices = nullptr;
	indices = nullptr;
}
