#ifndef XYZ_MODEL_HPP_
#define XYZ_MODEL_HPP_

#include <glm/glm.hpp>

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
};

class Model {
	public:
		virtual glm::vec3& intersect(const Ray&) =0;
};

#endif
