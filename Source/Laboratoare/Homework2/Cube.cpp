#include "Cube.h"

Cube::Cube(char *name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
	glm::vec3 color) : Object3D(name, position, rotation, scale, color)
{
	Init();
}

Cube::~Cube()
{
}

void Cube::Init()
{
	// This is not commented code. Is the way the vertices were chosen
	//VertexFormat(glm::vec3(-0.5f, -0.5f,  0.5f), color), // 0
	//VertexFormat(glm::vec3( 0.5f, -0.5f,  0.5f), color), // 1
	//VertexFormat(glm::vec3( 0.5f,  0.5f,  0.5f), color), // 2
	//VertexFormat(glm::vec3(-0.5f,  0.5f,  0.5f), color), // 3
	//VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), color), // 4
	//VertexFormat(glm::vec3( 0.5f, -0.5f, -0.5f), color), // 5
	//VertexFormat(glm::vec3( 0.5f,  0.5f, -0.5f), color), // 6
	//VertexFormat(glm::vec3(-0.5f,  0.5f, -0.5f), color), // 7

	this->vertices = {
		// Front face
		VertexFormat(glm::vec3(-0.5f, -0.5f,  0.5f), color, glm::vec3(0, 0, 1)), // 0
		VertexFormat(glm::vec3( 0.5f, -0.5f,  0.5f), color, glm::vec3(0, 0, 1)), // 1
		VertexFormat(glm::vec3( 0.5f,  0.5f,  0.5f), color, glm::vec3(0, 0, 1)), // 2
		VertexFormat(glm::vec3(-0.5f,  0.5f,  0.5f), color, glm::vec3(0, 0, 1)), // 3

		// Right face
		VertexFormat(glm::vec3( 0.5f, -0.5f,  0.5f), color, glm::vec3(1, 0, 0)), // 1
		VertexFormat(glm::vec3( 0.5f, -0.5f, -0.5f), color, glm::vec3(1, 0, 0)), // 5
		VertexFormat(glm::vec3( 0.5f,  0.5f, -0.5f), color, glm::vec3(1, 0, 0)), // 6
		VertexFormat(glm::vec3( 0.5f,  0.5f,  0.5f), color, glm::vec3(1, 0, 0)), // 2
		
		// Back face
		VertexFormat(glm::vec3( 0.5f, -0.5f, -0.5f), color, glm::vec3(0, 0, -1)), // 5
		VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), color, glm::vec3(0, 0, -1)), // 4
		VertexFormat(glm::vec3(-0.5f,  0.5f, -0.5f), color, glm::vec3(0, 0, -1)), // 7
		VertexFormat(glm::vec3( 0.5f,  0.5f, -0.5f), color, glm::vec3(0, 0, -1)), // 6

		// Left face
		VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), color, glm::vec3(-1, 0, 0)), // 4
		VertexFormat(glm::vec3(-0.5f, -0.5f,  0.5f), color, glm::vec3(-1, 0, 0)), // 0
		VertexFormat(glm::vec3(-0.5f,  0.5f,  0.5f), color, glm::vec3(-1, 0, 0)), // 3
		VertexFormat(glm::vec3(-0.5f,  0.5f, -0.5f), color, glm::vec3(-1, 0, 0)), // 7
		
		// Bottom face
		VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), color, glm::vec3(0, -1, 0)), // 4
		VertexFormat(glm::vec3( 0.5f, -0.5f, -0.5f), color, glm::vec3(0, -1, 0)), // 5
		VertexFormat(glm::vec3( 0.5f, -0.5f,  0.5f), color, glm::vec3(0, -1, 0)), // 1
		VertexFormat(glm::vec3(-0.5f, -0.5f,  0.5f), color, glm::vec3(0, -1, 0)), // 0

		// Top face
		VertexFormat(glm::vec3(-0.5f,  0.5f,  0.5f), color, glm::vec3(0, 1, 0)), // 3
		VertexFormat(glm::vec3( 0.5f,  0.5f,  0.5f), color, glm::vec3(0, 1, 0)), // 2
		VertexFormat(glm::vec3( 0.5f,  0.5f, -0.5f), color, glm::vec3(0, 1, 0)), // 6
		VertexFormat(glm::vec3(-0.5f,  0.5f, -0.5f), color, glm::vec3(0, 1, 0)), // 7
	};

	this->indices = {
		0, 1, 2,	0, 2, 3,
		4, 5, 6,	4, 6, 7,
		8, 9, 10,	8, 10, 11,
		12, 13, 14,	12, 14, 15,
		16, 17, 18,	16, 18, 19, 
		20, 21, 22, 20, 22, 23
	};
}
