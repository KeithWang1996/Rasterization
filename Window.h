#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#ifdef __APPLE__
// If modern OpenGL replace gl.h with gl3.h
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include "Cube.h"
#include "OBJObject.h"
class Window
{
public:
	static int model;
	static int width;
	static int height;
	static int render;
	static GLfloat* pixels;
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback( GLFWwindow* window);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void rasterize();
	static void drawPoint(int x, int y, float z, float r, float g, float b);
	static void cleanBuffer();
};

#endif
