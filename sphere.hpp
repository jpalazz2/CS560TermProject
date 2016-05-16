#ifndef XYZ_SPHERE_HPP_
#define XYZ_SPHERE_HPP_

#include "model.hpp"
#include "triangle.hpp"
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
		std::vector<Triangle> triangles;
		glm::vec3 center;
};t

#endif
