#pragma once

#include "Cylinder.h"
#include "WhiteBall.h"
#include "CollidableSphere.h"
#include <vector>

class Cue : public Cylinder {

public:
	Cue(char *name, glm::vec3 position, glm::vec3 rotation,
		glm::vec3 scale);
	~Cue();

private:
	static const glm::vec3 cueColor;
	WhiteBall *whiteBall;
	glm::vec3 cameraPosition;

	const float MAX_FORCE = 0.15f;
	float shootForce = 0;
	float cueBallDistance = 0;
	float deltaForce = 2.0f;
	float crtForce = 0;

public:
	bool isSticked = true;
	bool duringShoot = false;
	bool isShooting = false;
	void SetWhiteBall(WhiteBall *ball);
	void SetCameraPosition(glm::vec3 camPosition);
	float GetDistance();

	void Update(float gameTime);
	void HandleCollisionWithBalls(std::vector<CollidableSphere*> balls);

};