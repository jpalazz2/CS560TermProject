#ifndef XYZ_SPHERE_HPP_
#define XYZ_SPHERE_HPP_

#include "model.hpp"
#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>
#include <iostream>

class Sphere : public Model {
	public:
		Sphere() = delete;
		Sphere(const char *, const glm::vec3&);
		glm::vec3& intersect(const Ray&) {};

	public:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> faces;
		std::vector<glm::vec3> normals;
		std::unordered_map<int, int> vertexToNormalMap;
		glm::vec3 center;
};

#endif
