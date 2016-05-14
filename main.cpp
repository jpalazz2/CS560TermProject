#include <GLFW/glfw3.h>
#include <cstdlib>

int main(void) {

	if (!glfwInit()) {
		std::exit(EXIT_FAILURE);
	}

	return 0;
}
