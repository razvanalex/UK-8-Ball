#pragma once

#include "CollidableSphere.h"

class WhiteBall : public CollidableSphere {
public:
	WhiteBall(char *name, glm::vec3 position, glm::vec3 rotation,
		glm::vec3 scale, int sharpness);
	~WhiteBall();

public: 
	void Hit(float force, glm::vec3 velocity);
};