#include "sphere.hpp"

Sphere::Sphere(const char *fileName, const glm::vec3 &center): center(center) {
	std::ifstream inFile(fileName);
	if (!inFile.is_open()) {
		std::cerr << "Failed to open file: " << fileName << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::string line;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> faces;
	std::vector<glm::vec3> normals;
	std::unordered_map<int, int> vertexToNormalMap;
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
			vertices.push_back(glm::vec3{a, b, c});
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
	for (auto face : faces) {
		Triangle t;
		t.vertices[0] = vertices[face.x];
		t.normals[0] = normals[vertexToNormalMap[face.x]];
		t.vertices[1] = vertices[face.y];
		t.normals[1] = normals[vertexToNormalMap[face.y]];
		t.vertices[2] = vertices[face.z];
		t.normals[2] = normals[vertexToNormalMap[face.z]];
	}
}
