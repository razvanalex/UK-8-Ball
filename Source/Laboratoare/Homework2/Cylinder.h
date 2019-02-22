#pragma once

#include "Object3D.h"

class Cylinder : public Object3D {
	public:
		Cylinder(char *name, glm::vec3 position, glm::vec3 rotation, 
			glm::vec3 scale, glm::vec3 color, int sharpness, float radius1, 
			float radius2);
		~Cylinder();

	private:
		float radius1;
		float radius2;
		int sharpness;

		void Init() override;
};