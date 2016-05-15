#ifndef XYZ_KEYBOARD_HPP_
#define XYZ_KEYBOARD_HPP_

#include <GLFW/glfw3.h>

/*
 * Helper class to process keyboard input.
 */
class Keyboard {
	public:
		void process(GLFWwindow*, int, int, int, int);
};

#endif
