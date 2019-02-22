#include "Line.h"

Line::Line(char * name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, 
	glm::vec3 color) : Object3D(name, position, rotation, scale,  color)
{
	this->vertices = {
		VertexFormat(glm::vec3(-0.5f, 0, 0), color),
		VertexFormat(glm::vec3(0.5f, 0, 0), color)
	};

	this->indices = {
		0, 1
	};

	this->drawMode = GL_LINES;
}
