#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib>
#include <iostream>

#include "keyboard.hpp"
#include "shader.hpp"

static Keyboard kbd{};

void errorCallback(int, const char * description) {
	std::cerr << description << std::endl;
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	kbd.process(window, key, scancode, action, mods);
}

glm::vec3 lightPos{0.0f, 5.0f, -5.0f};
glm::vec3 cameraPos{0.0f, 0.0f, 0.0f};

int screenWidth = 512, screenHeight = 512;

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
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "CS 560 Term Project", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create the window!" << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Glew broke" << std::endl;
	}
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, keyboardCallback);

	// Otherwise OpenGL doesn't pay attention to depths when drawing 3d objects
	glEnable(GL_DEPTH_TEST);

	// Load, compile, and link our shaders
	Shader textureShader("shaders/texture.frag", "shaders/texture.vert");
	Shader tracerShader("shaders/model.frag", "shaders/model.vert");

	// Set a viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Draw a quad that fills the entire viewport
	// This gets drawn using the normalized device coordinates
	// We use it as the screen onto which we render the ray traced room
	GLfloat quadVertices[] = {
		-1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f
	};

	GLuint quadVBO, quadVAO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (GLvoid*) 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Lets make a texture to render our scene into
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create a framebuffer to help us render into the texture
	GLuint framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

  tracerShader.use();

	GLint lightLoc = glGetUniformLocation(tracerShader.shaderProgram, "light");
	glUniform3f(lightLoc, 0.4f, 0.5f, -0.6f);
	GLint sphere0Loc = glGetUniformLocation(tracerShader.shaderProgram, "sphere0");
	glUniform3f(sphere0Loc, -0.9f, -0.9f, -0.9f);
	GLint radius0Loc = glGetUniformLocation(tracerShader.shaderProgram, "radius0");
	glUniform1f(radius0Loc, 0.25f);
	GLint sphere1Loc = glGetUniformLocation(tracerShader.shaderProgram, "sphere1");
	glUniform3f(sphere1Loc, 0.0f, 0.25f, 0.0f);
	GLint radius1Loc = glGetUniformLocation(tracerShader.shaderProgram, "radius1");
	glUniform1f(radius1Loc, 0.25f);
	// While the window should still be showing...
	GLuint textures[] = {texture, texture2};
	while (!glfwWindowShouldClose(window)) {

		tracerShader.use();
		GLint textureWeightLoc = glGetUniformLocation(tracerShader.shaderProgram, "texture_weight");
		glUniform1f(textureWeightLoc, glfwGetTime() / (float) (glfwGetTime() + 1.0f));

		GLint timeLoc = glGetUniformLocation(tracerShader.shaderProgram, "time");
    glUniform1f(timeLoc, glfwGetTime());

		// Render our scene into the texture
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[1], 0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// Unbind our framebuffer and render our texture as a quad
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::swap(textures[0], textures[1]);

		textureShader.use();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
