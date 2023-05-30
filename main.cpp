#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// shader code
// shaders are openGL object
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\n";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.92f, 0.25f, 0.20f, 1.0f);\n"
"}\n\0";

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

	// create reference aka value to store vertex shader in

	// openGL version of unsigned integer
	//						specify we want vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//			  ref value,    1 screen, source code,   NULL
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// have to compile it for the GPU to understand this 
	glCompileShader(vertexShader);

	// same as above but fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// have to wrap in shader program
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// wrap up GL shader program
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// array of datatype GLfloat
	// every 3 floats will represent one coord

	// equilateral triangle
	GLfloat vertices[] =
	{
		// left corner
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		// right corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		// top corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
		// inner left
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // inner down
	};

	GLuint indices[] =
	{
		// triangles by points
		0, 3, 5, // lower left triangle
		3, 2, 4, // lower right triangle
		5, 4, 1 // upper triangle
	};


	// when you send info from CPU to GPU, you want to send it in big batches, called buffers 
	// not like front and end buffers below
	// so we create vertexBuffer obj
	// EBO is index buffer
	GLuint VAO, VBO, EBO;

	// ORDER MATTERS, MUST BE BEFORE VBO
	// VAOs allow you to quickly switch between different VBOs
	glGenVertexArrays(1, &VAO);

	// binding makes a certain object the current obj
	// if you make a function that modifies the certain object it modifies the current obj
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind VAO so we can work with it
	glBindVertexArray(VAO);

	// see other types of buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// store vertices
	// fourth arg can be (improves perf):
	// stream - vertices will be modified once and used a few times
	// static - vertices will be modified once and used many many times
	// dynamic - vertices will be modified multiple times and used many many times
	// also choose between:
	// draw - vertices will be modified and used to draw an image on the screen
	// read - vertices will be read
	// copy - vertices will be copied
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// make it current
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// configure VAO
	// 1. Position index of vertex attrib which allows us to communicate with the vertex shader from the outside
	// 2. number of values per vertex 
	// 3. datatype 
	// 4. only matters if coords are ints else GL_FALSE
	// 5. stride of our vertices - amt of data between each vertex so with 3 floats, then 3 * sizeof one float
	// 6. offset - pointer to where our vertices begin in the array, since its at the beginning we just do (void*)0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// enable it. 0 is position of vertex attribute
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// make sure EBO is unbound AFTER VAO since EBO is stored in VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	// swap the front buffer with back buffer
	glfwSwapBuffers(window);

	// dont close until given close input
	while (!glfwWindowShouldClose(window)) {
		/*
		REMEMBER, YOU HAVE TWO DISPLAYS, THE CURRENT VIEW AND THE BUFFER WHICH IS WHAT WILL BE RENDERED NEXT
	
		FRONT BUFFER AND BACK BUFFER
		*/

		// tell opengl to prepare to change the screen to next page
		glClearColor(0.93f, 1.0f, 0.99f, 1.0f);

		// execute command on color buffer... clean back buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// activate the shader program
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

		// (type of primitive, # of indices to draw, datatype of indice, starting index)
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

		// swap the buffers so the image gets updated each frame
		glfwSwapBuffers(window);

		// tell it to process the poll events, w/o it will stay in not responding forever
		glfwPollEvents();
	}

	// delete the objs we created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);


	// destroy the window
	glfwDestroyWindow(window);

	// terminate glfw
	glfwTerminate();

	return 0;
}
