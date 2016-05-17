#include "input.hpp"
#include <iostream>

void Input::processKeyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

void Input::processMouse(glm::vec3 &vec, GLFWwindow *window, double xpos, double ypos) {
	if (last.x == 0 || last.y == 0) {
		last.x = xpos;
		last.y = ypos;
		return;
	}
	vec.x += (last.x - xpos) / 200.0f;
	vec.y += (last.y - ypos) / 200.0f;
	last.x = xpos;
	last.y = ypos;
}
