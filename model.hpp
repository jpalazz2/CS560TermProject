#ifndef XYZ_MODEL_HPP_
#define XYZ_MODEL_HPP_

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Model {
	public:
		Model() = delete;
		Model(const char *);
		~Model();
		GLfloat *vertices;
		int verticesSize;
		GLuint *indices;
		int indicesSize;
};

#endif
