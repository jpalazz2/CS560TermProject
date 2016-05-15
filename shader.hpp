#ifndef _XYZ_SHADER_HPP_
#define _XYZ_SHADER_HPP_

#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdlib>

/*
 * Class to handle loading in our custom shaders.
 */
class Shader {
	public:
		Shader() = delete;
		Shader(const char *, const char *);
		GLuint shaderProgram;
	private:
		void loadFile(const GLuint &, const char *);
};

#endif
