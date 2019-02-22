#include "Cylinder.h"

#include <iostream>

Cylinder::Cylinder(char * name, glm::vec3 position, glm::vec3 rotation, 
	glm::vec3 scale, glm::vec3 color, int sharpness, float radius1, 
	float radius2) : Object3D(name, position, rotation, scale, color)
{
	this->sharpness = sharpness >= 3 ? sharpness : 3;
	this->radius1 = radius1;
	this->radius2 = radius2;

	Init();
}

Cylinder::~Cylinder()
{
}

void Cylinder::Init()
{
	float x, y, z;
	unsigned short crtIndex = 1, centerIndex = 0;
	float step = 2.0f * glm::pi<float>() / (float)sharpness;

	// Generate the 2 circles
	for (int c = 0; c < 2; c++) {
		float radius = (c == 0) ? radius1 : radius2;

		x = 0;
		y = (c == 0) ? -0.5f : 0.5f;
		z = 0;
		this->vertices.push_back(VertexFormat(glm::vec3(x, y, z), color,
			glm::vec3(0, y, 0)));

		for (float i = 0; i < 2 * glm::pi<float>(); i += step) {
			x = radius * cos(i);
			z = radius * sin(i);

			this->vertices.push_back(VertexFormat(glm::vec3(x, y, z), color,
				glm::vec3(0, y, 0)));

			if (crtIndex >= centerIndex + 2) {
				if (c == 1) {
					this->indices.push_back(crtIndex);
					this->indices.push_back(crtIndex - 1);
					this->indices.push_back(centerIndex);
				}
				else {
					this->indices.push_back(centerIndex);
					this->indices.push_back(crtIndex - 1);
					this->indices.push_back(crtIndex);
				}
			}

			crtIndex++;
		}

		if (c == 1) {
			this->indices.push_back(centerIndex + 1);
			this->indices.push_back(crtIndex - 1);
			this->indices.push_back(centerIndex);
		}
		else {
			this->indices.push_back(centerIndex);
			this->indices.push_back(crtIndex - 1);
			this->indices.push_back(centerIndex + 1);
		}
		centerIndex = crtIndex++;
	}

	// Generate the side of the cylinder
	crtIndex--;
	centerIndex = crtIndex;
	for (float i = 0; i < 2 * glm::pi<float>(); i += step)
	{
		x = radius1 * cos(i);
		z = radius1 * sin(i);

		this->vertices.push_back(VertexFormat(glm::vec3(x, -0.5f, z), color,
			glm::vec3(x, 0, z)));

		x = radius2 * cos(i);
		z = radius2 * sin(i);
		this->vertices.push_back(VertexFormat(glm::vec3(x, 0.5f, z), color,
			glm::vec3(x, 0, z)));

		if (crtIndex >= centerIndex + 2)
		{
			this->indices.push_back(crtIndex + 1);
			this->indices.push_back(crtIndex);
			this->indices.push_back(crtIndex - 2);

			this->indices.push_back(crtIndex - 1);
			this->indices.push_back(crtIndex + 1);
			this->indices.push_back(crtIndex - 2);
		}

		crtIndex += 2;
	}

	this->indices.push_back(centerIndex + 1);
	this->indices.push_back(centerIndex);
	this->indices.push_back(crtIndex - 2);

	this->indices.push_back(crtIndex - 1);
	this->indices.push_back(centerIndex + 1);
	this->indices.push_back(crtIndex - 2);
}
