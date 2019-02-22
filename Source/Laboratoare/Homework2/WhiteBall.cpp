#include "WhiteBall.h"

WhiteBall::WhiteBall(char *name, glm::vec3 position, glm::vec3 rotation,
	glm::vec3 scale, int sharpness) : 
	CollidableSphere(name,  position, rotation,
		scale, glm::vec3(1), sharpness)
{

}

WhiteBall::~WhiteBall()
{
}

void WhiteBall::Hit(float force, glm::vec3 velocity)
{
	this->velocity = 8 * force * glm::normalize(velocity);
}
