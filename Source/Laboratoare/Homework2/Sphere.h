#pragma once

#include <include/glm.h>
#include "Object3D.h"
#include <vector>
#include <Core/Engine.h>
#include "Transform3D.h"

class Sphere : public Object3D {
	public:
		Sphere(char *name, glm::vec3 position, glm::vec3 rotation,
			glm::vec3 scale, glm::vec3 color, int sharpness);
		~Sphere();

	private:
		int sharpness;
		const float initialRadius = 0.5f;

		void Init() override;
};