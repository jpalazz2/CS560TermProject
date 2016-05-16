#include "sphere.hpp"

Sphere::Sphere(const char *fileName, const glm::vec3 &center): center(center) {
	std::ifstream inFile(fileName);
	if (!inFile.is_open()) {
		std::cerr << "Failed to open file: " << fileName << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::string line;
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
			vertices.push_back(glm::vec3{a, b, c} + center);
		} else if (!operation.compare("f")) {
			std::string a, b, c;
			if (!(ss >> a >> b >> c)) {
				std::cerr << "Bad object file! " << fileName << std::endl;
				std::exit(EXIT_FAILURE);
			}
			auto intify = [&](std::string str) { 
				int vertexNum = std::stoi(str.substr(0, str.find_first_of("/"))) - 1;
				int normalNum = std::stoi(str.substr(str.find_last_of("/") + 1)) - 1;
				auto it = vertexToNormalMap.find(vertexNum);
				if (it != vertexToNormalMap.end()) {
					assert(normalNum == it->second);
				}
				vertexToNormalMap.insert({vertexNum, normalNum});
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
	assert(vertices.size() > 0);
	assert(normals.size() > 0);
	assert(vertexToNormalMap.size() > 0);
	colors.reserve(vertices.size());
}

void Sphere::buildVertexArray(GLfloat *arr) {
	for (std::size_t i = 0; i < vertices.size(); i++) {
		glm::vec3 vert = vertices[i];
		glm::vec3 color = colors[i];
		arr[6 * i] = vert.x;
		arr[6 * i + 1] = vert.y;
		arr[6 * i + 2] = vert.z;
		arr[6 * i + 3] = color.x;
		arr[6 * i + 4] = color.y;
		arr[6 * i + 5] = color.z;
	}
}

void Sphere::buildIndexArray(GLuint *arr) {
	for (std::size_t i = 0; i < faces.size(); i++) {
		arr[i * 3] = faces[i].x;	
		arr[i * 3 + 1] = faces[i].y;	
		arr[i * 3 + 2] = faces[i].z;	
	}
}
