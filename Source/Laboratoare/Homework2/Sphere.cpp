#include "Sphere.h"

#include <iostream>

Sphere::Sphere(char *name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, 
	glm::vec3 color, int sharpness) : Object3D(name, position, rotation, scale, color)
{
	this->sharpness = sharpness;

	Init();
}

Sphere::~Sphere()
{
}

void Sphere::Init()
{
	int lvls = sharpness;
	int num = (lvls + 1) * (lvls + 2) / 2;

	for (int j = 0; j < 8; j++) {
		// Compute rotation matrix for each octade
		glm::mat4 rotationOY = Transform3D::RotateOY((j % 4) * glm::half_pi<float>());
		glm::mat4 rotationOX = Transform3D::RotateOX((j / 4) * glm::pi<float>());
		glm::mat4 rotate = rotationOX * rotationOY;

		// Compute the coordinates of each vertex of triangle from octade
		glm::vec3 v1 = glm::vec3(glm::vec4(0, 0.5, 0, 1) * rotate);
		glm::vec3 v2 = glm::vec3(glm::vec4(-0.5, 0, 0.5, 1) * rotate);
		glm::vec3 v3 = glm::vec3(glm::vec4(0.5, 0, 0.5, 1) * rotate);

		int counter = 0, lvl = 0, index = 0;
		int a = j * num;

		// Devide the big triangle in small triangles
		for (int i = 0; i < num; i++) {

			// Compute the coordiantes of the point
			glm::vec3 u = glm::vec3((float)lvl / lvls) * (v2 - v1) + v1;
			u += normalize(v3 - v2) * (glm::length(v3 - v2) / (float)lvls) * (float)index;
			u *= initialRadius / glm::length(u);

			// Compute the normal
			glm::vec3 n = normalize(u);

			// Add the vertex
			this->vertices.push_back(VertexFormat(u, color, n));

			// Add indices
			if (index > 0) {
				this->indices.push_back(a);
				this->indices.push_back(lvl + a);
				this->indices.push_back(lvl + a + 1);
				a++;

				if (index < lvl) {
					unsigned short b = lvl * (lvl + 1) / 2 + index;
					this->indices.push_back(b + j * num);
					this->indices.push_back(a);
					this->indices.push_back(a - 1);
				}
			}

			// Go to next vertex from the same line or to the next line
			if (counter == 0)
				counter = ++lvl;
			else
				counter--;

			index = lvl - counter;
		}
	}
}