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

/*
 * Load in a model from an obj file for rendering.
 * 
 * Makes the following assumptions about the format of the obj file:
 * - Drawn using triangle polygons (i.e. 3 points per face)
 * - Face format is as follows: f x/x/x x/x/x x/x/x
 *
 * Other formats won't parse properly
 */
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
