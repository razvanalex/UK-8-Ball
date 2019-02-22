#include "Cue.h"


#include <iostream>
const glm::vec3 Cue::cueColor = glm::vec3(0.9f, 0.77f, 0.38f);

Cue::Cue(char *name, glm::vec3 position, glm::vec3 rotation,
	glm::vec3 scale) : Cylinder(name, position, rotation, glm::vec3(1), 
		cueColor, 10, 0.3f, 0.1f)
{
	this->scale = glm::vec3(.05f, 1, .05f) * scale;
	this->whiteBall = NULL;

	this->UpdateModelMatrix();
}

Cue::~Cue()
{
}

void Cue::SetWhiteBall(WhiteBall *ball)
{
	this->whiteBall = ball;
}

void Cue::SetCameraPosition(glm::vec3 camPosition) 
{
	this->cameraPosition = camPosition;
}

float Cue::GetDistance()
{
	return this->cueBallDistance;
}

void Cue::Update(float gameTime)
{
	// Compute vectors used at the rotation and position of the cue
	// according to the camera position.
	glm::vec3 direction = glm::normalize(whiteBall->GetPosition() - cameraPosition);
	direction = glm::vec3(direction.x, 0, direction.z);

	float distance = -this->scale.y / 2 - whiteBall->GetScale().x / 2;
	float height = whiteBall->GetPosition().y;

	this->position =
		glm::vec3(whiteBall->GetPosition().x, height, whiteBall->GetPosition().z) +
		distance * glm::normalize(
		glm::vec3(direction.x, 0, direction.z));

	this->rotation.x = glm::half_pi<float>();

	float angle = glm::atan((float)direction.z / direction.x);
	angle = direction.x >= 0 ? angle : glm::pi<float>() + angle;

	this->rotation.z = angle - glm::half_pi<float>();

	// Update the force applied to the ball
	if (isShooting) {
		// Choosing the force
		crtForce += deltaForce * gameTime;
		shootForce = MAX_FORCE - MAX_FORCE * glm::cos(crtForce);
		cueBallDistance = shootForce;
		duringShoot = true;
	}
	else if (duringShoot) {
		// Applying the force
		cueBallDistance -=  deltaForce * gameTime;
		if (cueBallDistance <= 0) {
			glm::vec3 direction = glm::vec3(glm::cos(angle), 0, glm::sin(angle));
			whiteBall->Hit(shootForce, direction);
			cueBallDistance = 0;
			shootForce = 0;
			crtForce = 0;
			duringShoot = false;
		}
	}

	this->UpdateModelMatrix();
}
