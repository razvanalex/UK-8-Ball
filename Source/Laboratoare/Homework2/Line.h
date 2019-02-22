#pragma once

#include "Object3D.h"

class Line : public Object3D {
public:
	Line(char *name, glm::vec3 position, glm::vec3 rotation,
		glm::vec3 scale, glm::vec3 color);
	~Line();
};