#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib>
#include <iostream>

#include "keyboard.hpp"
#include "shader.hpp"
#include "model.hpp"

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
	// Some setup for opengl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window and set glfw settings
	glfwSetErrorCallback(errorCallback);
	GLFWwindow* window = glfwCreateWindow(1280, 800, "CS 560 Term Project", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create the window!" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, keyboardCallback);

	// Load, compile, and link our shaders
	Shader s("shader/shader.frag", "shader/shader.vert");

	// Set a viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	/*
	GLfloat vertices[] = {
		0.5f,  0.5f, 2.0f,  // Top Right
		0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		-0.5f,  0.5f, 2.0f   // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};
	*/

	// Load in a sphere object
	Model sphere("objects/sphere.obj");

	// Vertex buffer and array objects in the CPU for use with our shader
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	// Write our list of vertices into the VBO buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.verticesSize * sizeof(GLfloat), sphere.vertices, GL_STATIC_DRAW);

	// Write our vertex indices used for drawing into the EBO buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.indicesSize * sizeof(GLuint), sphere.indices, GL_STATIC_DRAW);

	// Tell OpenGL what our vertex data looks like
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Unbind all buffers for safety
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// While the window should still be showing...
	while (!glfwWindowShouldClose(window)) {
		// Setup the orthographic projection
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

    // Tell OpenGL to use our shader
		glUseProgram(s.shaderProgram);

    // Projection matrices
    glm::mat4 model, view, projection;
    view = glm::lookAt(glm::vec3(0.0, 0.0, -5.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    projection = glm::perspective(45.0f, 1280 / 800.0f, 0.1f, 100.0f);

		// Where are these variables stored in memory
    GLint modelLoc = glGetUniformLocation(s.shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(s.shaderProgram, "view");
    GLint projectionLoc = glGetUniformLocation(s.shaderProgram, "projection");

		// Copy our projection matrix information into the correct locations in memory
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Use our vertex array object
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sphere.indicesSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
