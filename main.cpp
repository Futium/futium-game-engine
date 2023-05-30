#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
	
	// initiliaze glfw
	glfwInit();

	// tell glfw what version we are using through hint... 
	// in this case it is OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3.3);

	// only modern GLFW functions (CORE profile)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window				(width, height, "Name of Window", fullScreenOrNot, NULL
	GLFWwindow* window = glfwCreateWindow(800, 800, "Futium Game Engine", NULL, NULL);

	// error check the window
	if (window == NULL) {
		std::cout << "FAILED to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	// tell GLFW that we actually want to use it 
	glfwMakeContextCurrent(window);

	// load glad
	gladLoadGL();

	// tell it that we want the viewport to be, (x0, y0 to x1, y1)
	glViewport(0, 0, 800, 800);

	/*
	REMEMBER, YOU HAVE TWO DISPLAYS, THE CURRENT VIEW AND THE BUFFER WHICH IS WHAT WILL BE RENDERED NEXT
	
	FRONT BUFFER AND BACK BUFFER
	*/

	// tell opengl to prepare to change the screen to next page
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// execute command on color buffer... clean back buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// swap the front buffer with back buffer
	glfwSwapBuffers(window);

	// dont close until given close input
	while (!glfwWindowShouldClose(window)) {
		// tell it to process the poll events, w/o it will stay in not responding forever
		glfwPollEvents();
	}

	// destroy the window
	glfwDestroyWindow(window);

	// terminate glfw
	glfwTerminate();

	return 0;
}