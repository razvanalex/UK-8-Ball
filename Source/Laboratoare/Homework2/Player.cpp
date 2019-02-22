#include "Player.h"

const glm::vec3 Player::NO_HIT = glm::vec3(0.5f);

Player::Player()
{
	color = Player::NO_HIT;
	ballsScored = std::vector<CollidableSphere*>();
	firstBallHit = Player::NO_HIT;
	lastBall = false;
	wins = 0;
	looses = 0;
	numBallsScored = 0;
	faults = 0;
}

Player::~Player()
{
}
