#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <math.h>

class OBJObject
{
public:
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	glm::mat4 toWorld;

	GLfloat angle;
	GLint size;
	GLint size2;
	GLfloat xaxis;
	GLfloat yaxis; 
	GLfloat zaxis;
	glm::vec4 position;
	GLfloat oangle;
	GLfloat rangle;
	glm::vec3 scales;
	OBJObject(const char* filepath);

	void parse(const char* filepath);
	void draw();
	glm::mat4 spin(GLfloat);
	glm::mat4 move();
	glm::mat4 scale();
	glm::mat4 orbit(GLfloat degree);
	void update();
};

#endif