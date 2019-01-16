#include "Window.h"

const char* window_title = "GLFW Starter Project";


//Cube cube(5.0f);
int Window::width;
int Window::height;
int Window::model = 1;
int Window::render = 0;

OBJObject bunny("bunny.obj");
OBJObject bear("bear.obj");
OBJObject dragon("dragon.obj");

GLfloat* Window::pixels = new float[Window::width * Window::height * 3];
OBJObject *object;

using namespace std;



void Window::initialize_objects()
{
	object = &bunny;
}

void Window::clean_up()
{
	delete pixels;
}

void Window::cleanBuffer(){
	for (int i = 0; i<Window::width * Window::height; ++i)
	{
		pixels[i * 3] = 0.0f;
		pixels[i * 3 + 1] = 0.0f;
		pixels[i * 3 + 2] = 0.0f;
	}
}

void Window::drawPoint(int x, int y, float z, float r, float g, float b) {
	int offset = y * Window::width * 3 + x * 3;
	if ((x < Window::width) && (y < Window::height) && (x >= 0) && (y >= 0) && z<20) {
		pixels[offset] = r;
		pixels[offset + 1] = g;
		pixels[offset + 2] = b;
	}
}

void Window::rasterize() {
	for (unsigned int i = 0; i < object->normals.size(); ++i) {
		object->normals[i] = glm::normalize(object->normals[i]);
	}
	glm::mat4 M = object->toWorld;
	glm::mat4 Ci = lookAt(glm::vec3(0, 0, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 P = glm::perspective(glm::radians(60.0f), (float)Window::width / (float)Window::height, 1.0f, 1000.0f);
	glm::mat4 D = glm::mat4(glm::vec4(Window::width / 2, 0, 0, 0), glm::vec4(0, Window::height / 2, 0, 0),
		glm::vec4(0, 0, 0.5, 0), glm::vec4(Window::width / 2, Window::height / 2, 0.5, 1));
	for (unsigned int i = 0; i < object->vertices.size(); ++i) {
		glm::vec4 point = D*P*Ci*M*glm::vec4(object->vertices[i], 1);
		int x = (int)(point[0] / point[3]);
		int y = (int)(point[1] / point[3]);
		glm::vec4 vec = (object->toWorld)*(glm::vec4(object->vertices[i], 1));
		float z = vec[2];
		for (int j = x - (object->size2 - 1) / 2; j <= x + (object->size2 - 1) / 2; ++j) {
			for (int k = y - (object->size2 - 1) / 2; k <= y + (object->size2 - 1) / 2; ++k) {
					drawPoint(j, k, z,
						glm::normalize(object->normals[i]).x*0.5f + 0.5f,
						glm::normalize(object->normals[i]).y*0.5f + 0.5f,
						glm::normalize(object->normals[i]).z*0.5f + 0.5f);
			}
		}
	}
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);
	Window::height = height;
	Window::width = width;
	delete[] pixels;
	pixels = new float[Window::width * Window::height * 3];
	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	delete[] pixels;
	pixels = new float[Window::width * Window::height * 3];
	
	// Set the viewport size
	glViewport(0, 0, width, height);
	// Set the matrix mode to GL_PROJECTION to determine the proper camera properties
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// Set the perspective of the projection viewing frustum
	gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0);
	// Move camera back 20 units so that it looks at the origin (or else it's in the origin)
	glTranslatef(0, 0, -20);
	
}


void Window::idle_callback()
{
	// Perform any updates as necessary. Here, we will spin the cube slightly.
	//cube.update();
	object->update();
}


void Window::display_callback(GLFWwindow* window)
{
	
	if (render == 1) {
		cleanBuffer();
		if (model == 1)
			object = &bunny;
		if (model == 2)
			object = &dragon;
		if (model == 3)
			object = &bear;
		rasterize();
		glDrawPixels(Window::width, Window::height, GL_RGB, GL_FLOAT, pixels);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	else {
		// Clear the color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Set the matrix mode to GL_MODELVIEW
		glMatrixMode(GL_MODELVIEW);
		// Load the identity matrix
		glLoadIdentity();

		// Render objects
		//cube.draw();
		if (model == 1)
			object = &bunny;
		if (model == 2)
			object = &dragon;
		if (model == 3)
			object = &bear;

		object->draw();
		// Gets events, including input such as keyboard and mouse or window resizing
		glfwPollEvents();
		// Swap buffers
		glfwSwapBuffers(window);
	}
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		 
		if (key == GLFW_KEY_F1)
		{
			Window::model = 1;
		}
		if (key == GLFW_KEY_F2)
		{
			Window::model = 2;
		}
		if (key == GLFW_KEY_F3)
		{
			Window::model = 3;
		}
		if (key == GLFW_KEY_P) {
			if (mods & GLFW_MOD_SHIFT) {
				object->size = object->size + 1.0;
				object->size2 = object->size2 + 2.0;
			}
			else {
				if(object->size > 1.0f)
					object->size = object->size - 1.0;
				if(object->size2 > 1.0f)
				    object->size2 = object->size2 - 2.0;
			}
		}
		if (key == GLFW_KEY_X) {
			if (mods & GLFW_MOD_SHIFT)
				object->position = object->position + (object->orbit(-object->oangle)*glm::vec4(1, 0, 0, 1));
			else
				object->position = object->position - (object->orbit(-object->oangle)*glm::vec4(1, 0, 0, 1));
		}

		if (key == GLFW_KEY_Y) {
			if (mods & GLFW_MOD_SHIFT)
				object->position = object->position + (object->orbit(-object->oangle)*glm::vec4(0, 1, 0, 1));
			else
				object->position = object->position - (object->orbit(-object->oangle)*glm::vec4(0, 1, 0, 1));
		}

		if (key == GLFW_KEY_Z) {
			if (mods & GLFW_MOD_SHIFT)
				object->position = object->position + (object->orbit(-object->oangle)*glm::vec4(0, 0, 1, 1));
			else
				object->position = object->position - (object->orbit(-object->oangle)*glm::vec4(0, 0, 1, 1));
		}

		if (key == GLFW_KEY_S) {
			if (mods & GLFW_MOD_SHIFT)
				object->scales = object->scales*1.1f;
			else
				object->scales = object->scales/1.1f;
		}

		if (key == GLFW_KEY_O) {
			if (mods & GLFW_MOD_SHIFT)
				object->oangle = object->oangle - 10.0f;
			else
				object->oangle = object->oangle + 10.0f;
		}

		if (key == GLFW_KEY_R) {
			if (mods & GLFW_MOD_SHIFT) {
				object->rangle = -object->oangle;
				object->angle = 0.0f;
				object->scales = glm::vec3(1.0f, 1.0f, 1.0f);
			}
			else {
				object->position = glm::vec4(0, 0, 0, 1);
			}
		}
		if (key == GLFW_KEY_M) {
			if (render == 0) render = 1;
			else render = 0;
		}
	}
}
