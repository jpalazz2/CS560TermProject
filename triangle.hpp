#ifndef XYZ_TRIANGLE_HPP_
#define XYZ_TRIANGLE_HPP_

#include <glm/glm.hpp>

#include "model.hpp"

class Triangle : public Model {
	public:
		Triangle() {};
		glm::vec3& intersect(const Ray&) {};

	public:
		glm::vec3 vertices[3];
		glm::vec3 colors[3];
		glm::vec3 normals[3];
};

#endif
