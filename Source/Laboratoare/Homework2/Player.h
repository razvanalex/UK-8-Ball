#pragma once

#include <include/glm.h>
#include <vector>

#include "CollidableSphere.h"

class Player {
public: 
	Player();
	~Player();

public:
	glm::vec3 color;
	std::vector<CollidableSphere*> ballsScored;
	int numBallsScored;
	bool lastBall;
	glm::vec3 firstBallHit;
	int wins;
	int looses;
	int faults;

	static const glm::vec3 NO_HIT;
};