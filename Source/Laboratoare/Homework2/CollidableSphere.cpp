#include "CollidableSphere.h"
#include <iostream>

CollidableSphere::CollidableSphere(char * name, glm::vec3 position, 
	glm::vec3 rotation, glm::vec3 scale, glm::vec3 color, int sharpness)
	: Sphere(name, position, rotation, scale, color, sharpness)
{
	this->velocity = glm::vec3(0);
}

CollidableSphere::~CollidableSphere()
{
}

void CollidableSphere::Update(float gameTime)
{
	// Update the ball position
	float speed = GetSpeed() > 0 ? GetSpeed() - friction : 0;

	glm::vec3 vn = glm::normalize(velocity);
	velocity = glm::length(velocity) > friction ? speed * vn : glm::vec3(0);
	position += velocity * gameTime;

	// Update the model matrix
	this->UpdateModelMatrix();
}

bool CollidableSphere::CollideWith(CollidableSphere *sphere)
{
	// Compute the normal and the distance
	glm::vec3 normal = this->position - sphere->position;
	float minDistance = this->scale.x / 2 + sphere->scale.x / 2;
	float distance = glm::length(normal);
	normal = glm::normalize(normal);

	// If the balls are too far apart, do not collide
	if (distance > minDistance)
		return false;

	// If the balls are to close togather
	glm::vec3 minDist = normal * (minDistance - distance);
	position += minDist * 0.5f;
	sphere->position -= minDist * 0.5f;

	// Compute the tangent vector
	glm::vec3 tangent = glm::vec3(-normal.z, 0, normal.x);

	// Compute the components of both velocity vectors over N and T
	float v1n = glm::dot(normal, this->velocity);
	float v1t = glm::dot(tangent, this->velocity);
	float v2n = glm::dot(normal, sphere->velocity);
	float v2t = glm::dot(tangent, sphere->velocity);

	// Compute the new components
	glm::vec3 newV1n = v2n * normal;
	glm::vec3 newV1t = v1t * tangent;
	glm::vec3 newV2n = v1n * normal;
	glm::vec3 newV2t = v2t * tangent;

	// Set the new velocities
	this->velocity = (newV1n + newV1t) * (1 - energyLossFactor);
	sphere->velocity = (newV2n + newV2t) * (1 - energyLossFactor);

	return true;
}

void CollidableSphere::CollideWith(Cube *cube)
{
	// Compute the normal and the distance
	glm::vec3 newScale = cube->GetScale();
	glm::vec3 normal = glm::abs(position - cube->GetPosition()) - newScale / 2.0f;
	normal = glm::vec3(glm::max<float>(normal.x, 0), 0, glm::max<float>(normal.z, 0));
	float distance = glm::length(normal);

	// The sight of the normal
	float sign = glm::dot(normal, cube->GetPosition() - position) > 0 ? 1 : -1;

	float minDistance = this->scale.x / 2;
	normal = glm::normalize(sign * normal);

	// Check if the distance is 0, then use the velocity as normal
	if (distance == 0) {
		normal = glm::normalize(velocity);
	}

	// If the ball is far away of the rail or cushion, 
	//don't care... No collision
	if (distance > minDistance)
		return;

	// Compute de minimum distance between the ball and the cube
	glm::vec3 minDist = normal * (minDistance - distance);
	position -= minDist * 0.5f;
	
	// Compute the tangent vector
	glm::vec3 tangent = glm::vec3(-normal.z, 0, normal.x);

	float v1n = glm::dot(normal, this->velocity);
	float v1t = glm::dot(tangent, this->velocity);

	glm::vec3 newV1n = -v1n * normal;
	glm::vec3 newV1t = v1t * tangent;

	// Compute the new velocity vector
	this->velocity = (newV1n + newV1t) * (1 - energyLossFactor);
}

float CollidableSphere::GetSpeed()
{
	return glm::length(this->velocity);
}
