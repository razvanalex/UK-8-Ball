#pragma once

#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include <include/glm.h>

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>

#include "Camera.h"
#include "Table.h"
#include "Line.h"
#include "WhiteBall.h"
#include "CollidableSphere.h"
#include "Player.h"
#include "Cue.h"

using namespace std;

class Homework2 : public SimpleScene {
public:
	Homework2();
	~Homework2();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

	Mesh* CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices);
	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, glm::vec3 color, float materialKs);
	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, glm::vec3 color, float distance, bool isShooting, float materialKs);

	void Draw(float deltaTimeSeconds);

	void SetThirdPersonCam();
	void SetTopDownCam();
	bool MovingBalls();
	void HandleCollisions();
	bool CheckCollisions(glm::vec3 newPosition);
	void nextPlayer();
	void DrawStatistics();
	bool CheckForBalls();
	void ResetGame();

	const float MIN_CAM_DIST = 0.2f;
	const float MAX_CAM_DIST = 2.0f;

	glm::vec3 lightDirection;
	unsigned int materialShininess;
	float materialKd;
	float materialKs;

	Camera *myCam;
	glm::mat4 projectionMatrix;
	float camDistance;

	Table *table;
	Line *line;
	WhiteBall *whiteBall;
	Cue *cue;
	CollidableSphere *ball[15];
	vector<CollidableSphere*> collidableBalls;
	Player* players[2];

	glm::vec3 target = glm::vec3(0);

	glm::vec3 yellow = glm::vec3(1, 1, 0);
	glm::vec3 red = glm::vec3(1, 0, 0);
	glm::vec3 black = glm::vec3(0.1f);
	glm::vec3 colors[15] = { red, red, yellow, yellow, black, red, red, yellow, red, yellow, yellow, yellow, red, yellow, red };

	int crtPlayer;

	bool placeBall = true;
	bool startingBall = true;
	bool changeView = false;
	bool changeTurn = false;
	bool pocketBall = false;
	bool fault = false;
	bool whiteBallFault = false;
	bool colorsNotSet = true;

	glm::vec3 initPosBall;
};
