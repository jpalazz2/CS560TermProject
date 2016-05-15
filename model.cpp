#include "model.hpp"

Model::Model(const char *fileName) {
	std::ifstream inFile(fileName);
	if (!inFile.is_open()) {
		std::cerr << "Failed to open file: " << fileName << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::string line;
	std::vector<glm::vec3> objVertices;
	std::vector<glm::vec3> faces;
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
			auto intify = [](std::string str) { 
				return std::stoi(str.substr(0, str.find_first_of("/"))) - 1;
			};
			faces.push_back(glm::vec3{intify(a), intify(b), intify(c)});
		}
	}
	inFile.close();
	assert(faces.size() > 0);
	assert(objVertices.size() > 0);
	// Need space for three entries for each vertex
	vertices = new GLfloat[3 * objVertices.size()];
	// Each face entry requires 9 indices
	// 3 vertices per face, each of which require 3 indices
	indices = new GLuint[3 * faces.size()];
	verticesSize = 3 * objVertices.size();
	indicesSize = 3 * faces.size();
	for (std::size_t i = 0; i < objVertices.size(); i++) {
		glm::vec3 vec = objVertices[i];
		vertices[3 * i] = vec.x;
		vertices[3 * i + 1] = vec.y;
		vertices[3 * i + 2] = vec.z;
	}
	for (std::size_t i = 0; i < faces.size(); i++) {
		glm::vec3 vec = faces[i];
		indices[i * 3] = vec.x;
		indices[i * 3 + 1] = vec.y;
		indices[i * 3 + 2] = vec.z;
	}
}

Model::~Model() {
	delete[] vertices;
	delete[] indices;
	vertices = nullptr;
	indices = nullptr;
}
