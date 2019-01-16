#include "OBJObject.h"
#include <iostream>
#include <stdio.h>
using namespace std;

OBJObject::OBJObject(const char *filepath) 
{
	toWorld = glm::mat4(1.0f);
	OBJObject::size = 1;
	OBJObject::size2 = 1;
	OBJObject::xaxis = 0;
	OBJObject::yaxis = 0;
	OBJObject::zaxis = 0;
	OBJObject::position= glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	OBJObject::scales = glm::vec3(1.0f, 1.0f, 1.0f);
	OBJObject::oangle = 0;
	OBJObject::rangle = 0;
	parse(filepath);
}

void OBJObject::parse(const char *filepath) 
{
	FILE *fp;
	GLfloat x, y, z;
	GLfloat r, g, b;
	int c1, c2;
	fp = fopen(filepath, "rb");
	if (fp == NULL) {
		cerr<< "error loading file" <<endl; 
		exit(-1); 
	}
	
	while (!feof(fp)) {
		c1 = fgetc(fp);
		c2 = fgetc(fp);
		if ((c1 == 'v') && (c2 == ' ')) {
			fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			vertices.push_back(glm::vec3(x, y, z));
		}
		else if ((c1 == 'v') && (c2 == 'n')) {
			fscanf(fp, "%f %f %f", &x, &y, &z);
			normals.push_back(glm::vec3(x, y, z));
		}
	}
	fclose(fp);
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
}

void OBJObject::draw() 
{
	glMatrixMode(GL_MODELVIEW);

	// Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	glMultMatrixf(&(toWorld[0][0]));

	glBegin(GL_POINTS);
	//normalize to -1 to 1
	for (unsigned int i = 0; i < normals.size(); ++i) {
		normals[i] = glm::normalize(normals[i]);
	}
	
	for (unsigned int i = 0; i < vertices.size(); ++i) 
	{
		glColor3f(normals[i].x*0.5f + 0.5f, normals[i].y*0.5f + 0.5f, normals[i].z*0.5f + 0.5f);
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();

	// Pop the save state off the matrix stack
	// This will undo the multiply we did earlier
	glPopMatrix();
}

void OBJObject::update() {
	glPointSize(size);
	this->toWorld = orbit(this->oangle)*move()*orbit(this->rangle)*scale()*spin(1.0f);
}

glm::mat4 OBJObject::spin(GLfloat deg)
{
	this->angle += deg;
	if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
	// This creates the matrix to rotate the obj
	return (glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::mat4 OBJObject::orbit(GLfloat degree)
{
	if (degree >= 360.0f || degree <= -360.0f) this->oangle = 0.0f;
	glm::mat4 result = (glm::rotate(glm::mat4(1.0f), degree / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)));
	return result;
}

glm::mat4 OBJObject::move(){
	return glm::translate(glm::mat4(1.0f),glm::vec3(this->position));
}

glm::mat4 OBJObject::scale() {
	return glm::scale(glm::mat4(1.0f), scales);
}