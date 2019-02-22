#pragma once

#include <include/glm.h>
#include <vector>
#include "Object3D.h"
#include "Cube.h"
#include "Sphere.h"

class CollidableSphere : public Sphere {
	public:
		CollidableSphere(char *name, glm::vec3 position, glm::vec3 rotation,
			glm::vec3 scale, glm::vec3 color, int sharpness);
		~CollidableSphere();
	
	public:
		glm::vec3 velocity;
		const float friction = 0.005f;
		const float energyLossFactor = 0.02f;

		void Update(float gameTime);
		bool CollideWith(CollidableSphere * sphere);
		void CollideWith(Cube *cube);
		float GetSpeed();
};