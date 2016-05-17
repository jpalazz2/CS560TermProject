#ifndef XYZ_INPUT_HPP_
#define XYZ_INPUT_HPP_

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cmath>

/*
 * Helper class to process keyboard input.
 */
class Input {
	public:
		void processKeyboard(GLFWwindow*, int, int, int, int);
		void processMouse(glm::vec3&, GLFWwindow*, double, double);
	private:
		glm::vec2 last{0.0f};
};

#endif
