#pragma once

#include <include/glm.h>
#include "Object3D.h"
#include <vector>
#include <Core/Engine.h>
#include "Transform3D.h"

class Cube : public Object3D {
	public:
		Cube(char *name, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, glm::vec3 color);
		~Cube();

	private:
		void Init() override;
};