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
#include "model.hpp"

static Keyboard kbd{};

void errorCallback(int, const char * description) {
	std::cerr << description << std::endl;
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	kbd.process(window, key, scancode, action, mods);
}

glm::vec3 lightPos{-1.5f, 0.5f, -1.0f};
glm::vec3 cameraPos{0.0f, 0.0f, -5.0f};

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

	// Otherwise OpenGL doesn't pay attention to depths when drawing 3d objects
	glEnable(GL_DEPTH_TEST);

	// Load, compile, and link our shaders
	Shader modelShader("shader/model.frag", "shader/model.vert");
	Shader lightShader("shader/light.frag", "shader/light.vert");

	// Set a viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Load in a sphere object
	Model sphere("objects/sphere.obj");

	GLfloat plane[] = {
		-6.0f, -6.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		-6.0f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		6.0f, -6.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		6.0f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f
	};

	// Vertex buffer and array objects in the CPU for use with our shader
	GLuint VBO, VAO, EBO, lightVAO, planeVBO, planeVAO;
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &lightVAO);
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(VAO);

	// Write our list of vertices into the VBO buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.verticesSize * sizeof(GLfloat), sphere.vertices, GL_STATIC_DRAW);

	// Write our vertex indices used for drawing into the EBO buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.indicesSize * sizeof(GLuint), sphere.indices, GL_STATIC_DRAW);

	// Tell OpenGL what our vertex data looks like
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Also tell it how our normal data looks
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Now we need to setup vertex data for the light source
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.verticesSize * sizeof(GLfloat), sphere.vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.indicesSize * sizeof(GLuint), sphere.indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// While the window should still be showing...
	while (!glfwWindowShouldClose(window)) {
		float rotateAmount = glfwGetTime();
		glm::vec3 renderLightPos = glm::rotateY(lightPos, rotateAmount);
		// Setup the orthographic projection
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Tell OpenGL to use our shader
		modelShader.use();

    // Projection matrices
    glm::mat4 model, view, projection;
    view = glm::lookAt(cameraPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    projection = glm::perspective(45.0f, 1280 / 800.0f, 0.1f, 100.0f);

		// Where are these variables stored in memory
    GLint modelLoc = glGetUniformLocation(modelShader.shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(modelShader.shaderProgram, "view");
    GLint projectionLoc = glGetUniformLocation(modelShader.shaderProgram, "projection");

		// Tell the shader where our light is located
		GLint lightPosLoc = glGetUniformLocation(modelShader.shaderProgram, "lightPos");
		glUniform3f(lightPosLoc, renderLightPos.x, renderLightPos.y, renderLightPos.z);

		// Where are we viewing from? (for specular lighting)
	  GLint viewPosLoc = glGetUniformLocation(modelShader.shaderProgram, "viewPos");
	  glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);

    // Lets set some color information
    GLint sphereColorLoc = glGetUniformLocation(modelShader.shaderProgram, "objectColor");
    GLint lightColorLoc = glGetUniformLocation(modelShader.shaderProgram, "lightColor");
    glUniform3f(sphereColorLoc, 1.0f, 0.5f, 0.31f);
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

		// Copy our projection matrix information into the correct locations in memory
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Use our vertex array object
		glBindVertexArray(VAO);
		model = glm::translate(model, glm::vec3{1.0f, 1.0f, 1.0f});
		model = glm::scale(model, glm::vec3{0.5f, 0.5f, 0.5f});
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, sphere.indicesSize, GL_UNSIGNED_INT, 0);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3{-1.0f, -1.0f, -1.0f});
		model = glm::scale(model, glm::vec3{0.5f, 0.5f, 0.5f});
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawElements(GL_TRIANGLES, sphere.indicesSize, GL_UNSIGNED_INT, 0);

		glBindVertexArray(planeVAO);
		// back
		model = glm::mat4();
		model = glm::translate(model, glm::vec3{0.0f, 0.0f, 3.0f});
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// left
		model = glm::mat4();
		model = glm::translate(model, glm::vec3{3.0f, 0.0f, 0.0f});
		model = glm::rotate(model, 90.f, glm::vec3{0.0f, 1.0f, 0.0f});
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// right
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90.f, glm::vec3(0.0f, 1.0f, 0.0f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		
		glBindVertexArray(0);


		// Now we need to draw the light object
		lightShader.use();

    modelLoc = glGetUniformLocation(lightShader.shaderProgram, "model");
    viewLoc = glGetUniformLocation(lightShader.shaderProgram, "view");
    projectionLoc = glGetUniformLocation(lightShader.shaderProgram, "projection");

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    model = glm::mat4();
    model = glm::translate(model, renderLightPos);
    model = glm::scale(model, glm::vec3(0.3f));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(lightVAO);
		glDrawElements(GL_TRIANGLES, sphere.indicesSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
