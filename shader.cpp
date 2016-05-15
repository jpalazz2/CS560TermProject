#include "shader.hpp"

Shader::Shader(const char *fragment, const char *vertex) {
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	loadFile(vertexShader, vertex);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	loadFile(fragmentShader, fragment);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar info[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, info);
		std::cout << info << std::endl;
		std::exit(EXIT_FAILURE);
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
}

void Shader::loadFile(const GLuint &shader, const char *fileName) {
	std::ifstream inFile(fileName);
	std::string contents;
	if (inFile) {
		inFile.seekg(0, std::ios::end);
		contents.resize(inFile.tellg());
		inFile.seekg(0, std::ios::beg);
		inFile.read(&contents[0], contents.size());
		inFile.close();
	}
	const GLchar * tmp = contents.c_str();
	glShaderSource(shader, 1, &tmp, nullptr);
	glCompileShader(shader);
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar info[512];
		glGetShaderInfoLog(shader, 512, nullptr, info);
		std::cout << info << std::endl;
		std::exit(EXIT_FAILURE);
	}
}
