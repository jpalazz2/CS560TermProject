#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

#include "keyboard.hpp"

static Keyboard kbd{};

void errorCallback(int, const char * description) {
	std::cerr << description << std::endl;
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	kbd.process(window, key, scancode, action, mods);
}


int main(void) {

	if (!glfwInit()) {
		std::exit(EXIT_FAILURE);
	}

	glfwSetErrorCallback(errorCallback);
	GLFWwindow* window = glfwCreateWindow(1280, 800, "CS 560 Term Project", NULL, NULL);
	if (!window) {
		std::cerr << "Failed to create the window!" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, keyboardCallback);

	while (!glfwWindowShouldClose(window))
	{
		/*
		 * Setup the orthographic projection
		 */
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
