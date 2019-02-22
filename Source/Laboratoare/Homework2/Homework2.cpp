#include "Homework2.h"

Homework2::Homework2()
{
	myCam = new Camera();
	myCam->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0), glm::vec3(0, 1, 0));
	camDistance = 0.4f;

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

Homework2::~Homework2()
{
}

void Homework2::Init()
{
	// Initialize table
	{
		table = new Table(.5f, .075f, 0.1f, 0.05f, 0.05f, glm::vec3(1, 0.25f, 2), glm::vec3(0));

		for (auto e : table->GetObjects3D()) {
			AddMeshToList(e->CreateMesh());
		}

		line = new Line("line", glm::vec3(0, 0.5f + 0.225f + 0.05f, table->GetPlayableSize().y / 6),
			glm::vec3(0), glm::vec3(table->GetPlayableSize().x), glm::vec3(1));
		AddMeshToList(line->CreateMesh());
	}

	// Initialize the cue
	{
		cue = new Cue("cue", glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(1));
		AddMeshToList(cue->CreateMesh());
	}

	// Initialize the balls
	{
		// Init the white ball
		initPosBall = glm::vec3(0, 0.5f + 0.225f + 0.05f, table->GetPlayableSize().y / 4);
		whiteBall = new WhiteBall("sphere", initPosBall, glm::vec3(0), glm::vec3(0.05f), 10);
		AddMeshToList(whiteBall->CreateMesh());

		// Init the 15 balls
		glm::vec3 ballScale = glm::vec3(0.05f);
		float triangleSize = ballScale.x * (2 * (float)glm::sqrt(3) + 1);
		glm::vec3 position = glm::vec3(0, 0.5f + 0.225f + 0.05f,
			-(table->GetPlayableSize().y / 3 - triangleSize) / 2 - table->GetPlayableSize().y / 6);

		int index = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j <= i; j++) {
				char *name = new char[15];
				sprintf(name, "ball_%d", index);

				glm::vec3 ballPos = position - i * ballScale.x *
					glm::vec3(glm::cos(RADIANS(60)), 0, glm::sin(RADIANS(60))) + glm::vec3(j * ballScale.x, 0, 0);

				ball[index] = new CollidableSphere(name, ballPos, glm::vec3(0), ballScale, colors[index], 10);
				AddMeshToList(ball[index]->CreateMesh());
				index++;
			}
		}

		// Add all balls as being collidable
		collidableBalls = vector<CollidableSphere*>();
		collidableBalls.push_back(whiteBall);
		for (auto b : ball) {
			collidableBalls.push_back(b);
		}
	}

	// Init both players and draw statistics
	{
		for (int i = 0; i < 2; i++) {
			players[i] = new Player();
		}
		crtPlayer = 0;
		DrawStatistics();
	}
	
	// Set lighting properties
	{
		lightDirection = glm::vec3(0, -4, -7);
		materialShininess = 30;
		materialKd = 0.6;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("MyShader");
		shader->AddShader("Source/Laboratoare/Homework2/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Homework2/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

void Homework2::FrameStart()
{

}

void Homework2::Update(float deltaTimeSeconds)
{
	// Handle the collisions
	HandleCollisions();

	// Update the cue
	cue->SetCameraPosition(myCam->GetPosition());
	cue->SetWhiteBall(whiteBall);
	cue->Update(deltaTimeSeconds);

	// Update the balls
	for (auto b : collidableBalls)
		b->Update(deltaTimeSeconds);

	// Check for fault
	if (!cue->isSticked && !changeView && !MovingBalls() && !placeBall &&
		((players[crtPlayer]->firstBallHit == Player::NO_HIT)	// No ball touched
			|| (!colorsNotSet && players[crtPlayer]->color != Player::NO_HIT
				&& players[crtPlayer]->firstBallHit != players[crtPlayer]->color))) // Illegal ball touched
	{
		if (!colorsNotSet && !CheckForBalls()) {
			players[crtPlayer]->color = black;
			players[crtPlayer]->lastBall = true;
		}
		else {
			cout << ((players[crtPlayer]->firstBallHit == Player::NO_HIT) ? "No ball touched" : "Illegal ball touched") << endl;
			fault = true;
		}
	}

	// Check for next turn
	if (!changeTurn && (fault && !MovingBalls() || !pocketBall && !MovingBalls() && !cue->isSticked && !placeBall && !changeView)) {
		if (colorsNotSet && fault)
			players[crtPlayer]->color = Player::NO_HIT;

		changeTurn = true;
	} 

	// White ball pocketed
	if (whiteBallFault && !MovingBalls()) {
		collidableBalls.push_back(whiteBall);
		whiteBallFault = false;

		cout << "Illegal white ball pocketed" << endl;
	}

	// Unset change view
	if (changeView) {
		changeView = false;
	}

	// Set the camera and draw statistics
	if (placeBall || MovingBalls()) {
		SetTopDownCam();
	}
	else {
		if (!cue->isSticked) {
			if (!fault && colorsNotSet && players[crtPlayer]->color != Player::NO_HIT) {
				colorsNotSet = false;
			}
			if (!fault && !changeTurn)
				DrawStatistics();
			players[crtPlayer]->firstBallHit = Player::NO_HIT;
			SetThirdPersonCam();
			pocketBall = false;
			if (startingBall) {
				startingBall = false;
			}
		}

		cue->isSticked = true;
	}

	// Change the turn
	if (changeTurn) {	
		placeBall = fault;
		players[crtPlayer]->faults = (fault ? players[crtPlayer]->faults  + 1 : players[crtPlayer]->faults);
		players[crtPlayer]->firstBallHit = Player::NO_HIT;

		fault = false;
		pocketBall = false;
		changeTurn = false;

		if (!colorsNotSet && !CheckForBalls()) {
			players[crtPlayer]->color = black;
			players[crtPlayer]->lastBall = true;
		}

		nextPlayer();
		if (!placeBall && !MovingBalls() && cue->isSticked)
			SetThirdPersonCam();

		DrawStatistics();
	}

	// Draw all object on the screen
	Draw(deltaTimeSeconds);
}

void Homework2::Draw(float deltaTimeSeconds)
{
	// Set cull ccw
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Get the resolution
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	glClearColor(0, 0, 0, 1);

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
	
	// Draw the table
	for (auto e : table->GetObjects3D()) {
		RenderSimpleMesh(
			meshes[e->GetName()],
			shaders["MyShader"], e->GetModelMatrix(),
			e->GetColor(), 0.05);
	}

	// Draw the white line
	if (placeBall && startingBall) {
		RenderSimpleMesh(
			meshes[line->GetName()],
			shaders["VertexColor"], line->GetModelMatrix(),
			line->GetColor(), 0.5);
	}

	// Draw all balls that are on the table
	for (auto b : collidableBalls) {
		RenderSimpleMesh(
			meshes[b->GetName()],
			shaders["MyShader"], b->GetModelMatrix(),
			b->GetColor(), 0.5);
	}

	// Draw the cue
	if (cue->isSticked)
		RenderSimpleMesh(meshes[cue->GetName()], shaders["MyShader"], cue->GetModelMatrix(), 
			cue->GetColor(), cue->GetDistance(), cue->duringShoot, 0.2);
}

void Homework2::SetThirdPersonCam()
{
	float distance = table->GetPlayableSize().y * 2;

	// Find the nearast ball of player's color
	for (auto b : collidableBalls) {
		if (b == whiteBall)
			continue;

		float tmp_dist = glm::distance(b->GetPosition(), whiteBall->GetPosition());
		if (tmp_dist < distance
			&& (players[crtPlayer]->color == b->GetColor() || players[crtPlayer]->color == Player::NO_HIT))
		{
			glm::vec3 dif = b->GetPosition() - whiteBall->GetPosition();
			dif = glm::normalize(dif);

			target = glm::vec3(whiteBall->GetPosition().x, 0, whiteBall->GetPosition().z) +
					glm::vec3(dif.x, whiteBall->GetPosition().y / 4, dif.z);

			distance = tmp_dist;
		}
	}

	// Set the camera to third person, targeted to the nearast ball that 
	// has the same color as the player.
	myCam->SetTargetPosition(target, whiteBall->GetPosition(), camDistance);
}

void Homework2::SetTopDownCam()
{
	// Set the camera to be Top-Down
	myCam->SetTargetPosition(glm::vec3(-0.000001f, 0, 0), glm::vec3(0, 1, 0), 1);
	cue->isSticked = false;
}

bool Homework2::MovingBalls()
{
	// Check for moving balls
	bool moving = false;
	for (auto b : collidableBalls) {
		if (b->GetSpeed() != 0) {
			return true;
		}
	}

	return false;
}

void Homework2::HandleCollisions()
{
	// Handle collisions between balls
	for (int i = 0; i < collidableBalls.size(); i++) {
		for (int j = i + 1; j < collidableBalls.size(); j++) {
			bool result = collidableBalls.at(i)->CollideWith(collidableBalls.at(j));

			// Check for fault
			if (WhiteBall *ball = dynamic_cast<WhiteBall*>(collidableBalls.at(i))) {
				if (result && players[crtPlayer]->firstBallHit == Player::NO_HIT) {
					players[crtPlayer]->firstBallHit = collidableBalls.at(j)->GetColor();
				}
			} 
			else if (WhiteBall *ball = dynamic_cast<WhiteBall*>(collidableBalls.at(j))) {
				if (result && players[crtPlayer]->firstBallHit == Player::NO_HIT) {
					players[crtPlayer]->firstBallHit = collidableBalls.at(i)->GetColor();
				}
			}
		}
	}

	// Handle collisions between balls and table
	for (int i = 0; i < collidableBalls.size(); i++)
		for (auto p : table->GetCollidableObjects())
			collidableBalls.at(i)->CollideWith(p);

	// Check if a ball gets into a hole
	for (auto b : collidableBalls) {
		for (auto h : table->GetHoles()) {
			if (glm::length(h->GetPosition() - b->GetPosition()) < h->GetScale().x / 2) {
				collidableBalls.erase(std::remove(collidableBalls.begin(), collidableBalls.end(), b), collidableBalls.end());

				if (WhiteBall *whiteB = dynamic_cast<WhiteBall*>(b)) {
					// White ball pocketed
					whiteB->SetPosition(initPosBall);
					whiteB->velocity = glm::vec3(0);
					placeBall = true;
					fault = true;
					whiteBallFault = true;
				}
				else if (b->GetColor() != black) {
					// Any ball that is not white or black is pocketed
					if (!fault && players[crtPlayer]->color == Player::NO_HIT) {
						pocketBall = true;
						players[crtPlayer]->color = b->GetColor();
						players[1 - crtPlayer]->color = (b->GetColor() == yellow) ? red : yellow;
						players[crtPlayer]->numBallsScored++;
					}
					else if (players[crtPlayer]->color == b->GetColor()) {
						pocketBall = true;
						players[crtPlayer]->numBallsScored++;
					}

					players[crtPlayer]->ballsScored.push_back(b);
				}
				else {
					// Black ball is pocketed
					players[crtPlayer]->ballsScored.push_back(b);
					if (players[crtPlayer]->lastBall) {
						players[crtPlayer]->wins++;
						players[1 - crtPlayer]->looses++;
						cout << "Player " << crtPlayer + 1 << " has won!" << endl;
					}
					else { 
						players[1 - crtPlayer]->wins++;
						players[crtPlayer]->looses++;
						cout << "Player " << 2 - crtPlayer << " has won!" << endl;
					}

					ResetGame();
				}
			}	
		}
	}
}

bool Homework2::CheckCollisions(glm::vec3 newPosition)
{
	// Handle collisions between white ball and the others in 
	// positioning mode
	for (int i = 0; i < collidableBalls.size(); i++) {
		if (collidableBalls.at(i) == whiteBall)
			continue;

		float distance = collidableBalls.at(i)->GetScale().x / 2 + whiteBall->GetScale().x / 2;
		if (glm::distance(newPosition, collidableBalls.at(i)->GetPosition()) < distance)
			return true;
	}

	return false;
}

void Homework2::nextPlayer()
{
	// Change the player
	crtPlayer = 1 - crtPlayer;
}

void Homework2::DrawStatistics()
{
	// Draw all statistics
	glm::vec3 color = players[crtPlayer]->color;
	cout << "================================================" << endl;
	cout << "Current player: " << crtPlayer + 1 << endl
		 << "Wins: " << players[crtPlayer]->wins << endl
		 << "Looses: " << players[crtPlayer]->looses << endl
		 << "Faults: " << players[crtPlayer]->faults << endl
		 << "Number of balls scored: " << players[crtPlayer]->numBallsScored << endl
		 << "Balls pocketed: ";

	auto ballsScored = players[crtPlayer]->ballsScored;
	for (auto i = ballsScored.begin(); i != ballsScored.end(); i++) {
		glm::vec3 crtBallColor = (*i)->GetColor();
		std::cout << (crtBallColor == black ? "black" : (crtBallColor == yellow) ? "yellow" : "red") << ' ';
	}
	cout << endl;

	cout << "Current ball color: "
		<< (color == Player::NO_HIT ? "No color chosen" : (color == yellow) ? "yellow" : (color == red) ? "red" : "black") << endl;
	
}

bool Homework2::CheckForBalls()
{
	// Check if there are balls on the table of the player's color
	for (auto b : collidableBalls) {
		if (players[crtPlayer]->color != Player::NO_HIT && b->GetColor() == players[crtPlayer]->color)
			return true;
	}
	return false;
}

void Homework2::ResetGame()
{
	// Stop all the balls
	whiteBall->velocity = glm::vec3(0);
	for (auto b : ball) {
		b->velocity = glm::vec3(0);
	}

	// Reset white ball position
	whiteBall->SetPosition(initPosBall);

	// Reset the balls in triangle
	glm::vec3 ballScale = glm::vec3(0.05f);
	float triangleSize = ballScale.x * (2 * (float)glm::sqrt(3) + 1);
	glm::vec3 position = glm::vec3(0, 0.5f + 0.225f + 0.05f,
		-(table->GetPlayableSize().y / 3 - triangleSize) / 2 - table->GetPlayableSize().y / 6);

	int index = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j <= i; j++) {
			glm::vec3 ballPos = position - 
				i * ballScale.x * glm::vec3(glm::cos(RADIANS(60)), 0, glm::sin(RADIANS(60))) + glm::vec3(j * ballScale.x, 0, 0);
			
			ball[index]->SetPosition(ballPos);
			index++;
		}
	}
	
	// Add all balls to collidable objects
	collidableBalls.clear();

	collidableBalls.push_back(whiteBall);
	for (auto b : ball) {
		collidableBalls.push_back(b);
	}

	// Reset the players
	crtPlayer = 0;
	for (int i = 0; i < 2; i++) {
		players[i]->color = Player::NO_HIT;
		players[i]->firstBallHit = Player::NO_HIT;
		players[i]->lastBall = false;
		players[i]->numBallsScored = 0;
		players[i]->ballsScored.clear();
		players[i]->faults = 0;
	}

	// Reset all other vaiables
	placeBall = true;
	startingBall = true;
	changeView = false;
	changeTurn = false;
	pocketBall = false;
	fault = false;
	whiteBallFault = false;
	colorsNotSet = true;
}

void Homework2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, glm::vec3 color, 
	float materialKs)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);
	// Get and set shader location for uniform mat4 "Model"
	int location = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Get and set shader location for uniform mat4 "View"
	location = glGetUniformLocation(shader->program, "View");
	glm::mat4 viewMatrix = myCam->GetViewMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Get and set shader location for uniform mat4 "Projection"
	location = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Get and set shader location for uniform vec3 "color"
	location = glGetUniformLocation(shader->program, "color");
	glUniform3fv(location, 1, glm::value_ptr(color));

	// Get and set shader location for uniform int "isShooting"
	location = glGetUniformLocation(shader->program, "isShooting");
	glUniform1i(location, 0);

	// Get and set shader location for uniform vec3 "light_direction"
	location = glGetUniformLocation(shader->program, "light_direction");
	glUniform3fv(location, 1, glm::value_ptr(lightDirection));

	// Get and set shader location for uniform vec3 "eye_position"
	location = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(location, 1, glm::value_ptr(myCam->GetPosition()));

	// Get and set shader location for uniform float "material_kd"
	location = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(location, materialKd);

	// Get and set shader location for uniform float "material_ks"
	location = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(location, materialKs);

	// Get and set shader location for uniform int "material_shininess"
	location = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(location, materialShininess);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Homework2::RenderSimpleMesh(Mesh *mesh, Shader *shader,
	const glm::mat4 & modelMatrix, glm::vec3 color,
	float distance, bool isShooting, float materialKs)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// Render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Get and set shader location for uniform mat4 "Model"
	int location = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Get and set shader location for uniform mat4 "View"
	location = glGetUniformLocation(shader->program, "View");
	glm::mat4 viewMatrix = myCam->GetViewMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Get and set shader location for uniform mat4 "Projection"
	location = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Get and set shader location for uniform vec3 "color"
	location = glGetUniformLocation(shader->program, "color");
	glUniform3fv(location, 1, glm::value_ptr(color));

	// Get and set shader location for uniform float "distance"
	location = glGetUniformLocation(shader->program, "distance");
	glUniform1f(location, distance);

	// Get and set shader location for uniform int "isShooting"
	location = glGetUniformLocation(shader->program, "isShooting");
	glUniform1i(location, isShooting ? 1 : 0);

	// Get and set shader location for uniform vec3 "light_direction"
	location = glGetUniformLocation(shader->program, "light_direction");
	glUniform3fv(location, 1, glm::value_ptr(lightDirection));

	// Get and set shader location for uniform vec3 "eye_position"
	location = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(location, 1, glm::value_ptr(myCam->GetPosition()));

	// Get and set shader location for uniform float "material_kd"
	location = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(location, materialKd);

	// Get and set shader location for uniform float "material_ks"
	location = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(location, materialKs);

	// Get and set shader location for uniform int "material_shininess"
	location = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(location, materialShininess);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

Mesh* Homework2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}

void Homework2::FrameEnd()
{
	DrawCoordinatSystem(myCam->GetViewMatrix(), projectionMatrix);
}

void Homework2::OnInputUpdate(float deltaTime, int mods)
{
	if (placeBall) {
		// Move the ball using the arrows
		float ballSpeed = .5f;
		float step = ballSpeed * deltaTime;

		if (window->KeyHold(GLFW_KEY_W)) {
			glm::vec3 newPos = whiteBall->GetPosition() + glm::vec3(-step, 0, 0);
			if ((newPos.x >= table->GetPosition().x - table->GetPlayableSize().x / 2 + whiteBall->GetScale().x / 2)
				&& !CheckCollisions(newPos))
			{
				whiteBall->SetPosition(newPos);
			}
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			glm::vec3 newPos = whiteBall->GetPosition() + glm::vec3(step, 0, 0);
			if ((newPos.x <= table->GetPosition().x + table->GetPlayableSize().x / 2 - whiteBall->GetScale().x / 2)
				&& !CheckCollisions(newPos))
			{
				whiteBall->SetPosition(newPos);
			}
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			glm::vec3 newPos = whiteBall->GetPosition() + glm::vec3(0, 0, step);
			if ((newPos.z <= table->GetPosition().z + table->GetPlayableSize().y / 2 - whiteBall->GetScale().x / 2)
				&& !CheckCollisions(newPos))
			{
				whiteBall->SetPosition(newPos);
			}
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			glm::vec3 newPos = whiteBall->GetPosition() + glm::vec3(0, 0, -step);
			if ((newPos.z >= table->GetPosition().z - table->GetPlayableSize().y / 2 + whiteBall->GetScale().x / 2)
				&& !CheckCollisions(newPos))
			{
				float firstThird = table->GetPlayableSize().y / 6;
				if ((startingBall && newPos.z >= firstThird) || !startingBall) {
					whiteBall->SetPosition(newPos);
				}
			}
		}
		
		if (window->KeyHold(GLFW_KEY_SPACE)) {
			// Change the view
			placeBall = false;
			changeView = true;
		}
	}
	else {
		float speed = 1.0f;
		// Change the distance between camera and white ball
		if (window->KeyHold(GLFW_KEY_Q)) {
			camDistance -= speed * deltaTime;
			camDistance = glm::clamp(camDistance, MIN_CAM_DIST, MAX_CAM_DIST);
			myCam->SetTargetPosition(myCam->GetTargetPosition(), whiteBall->GetPosition(), camDistance);
		}
		else if (window->KeyHold(GLFW_KEY_E)) {
			camDistance += speed * deltaTime;
			camDistance = glm::clamp(camDistance, MIN_CAM_DIST, MAX_CAM_DIST);
			myCam->SetTargetPosition(myCam->GetTargetPosition(), whiteBall->GetPosition(), camDistance);
		}
	}

	// Shoot the ball
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT))
	{
		if (!placeBall && !cue->duringShoot && !MovingBalls())
			cue->isShooting = true;
	}
	else {
		if (cue->isShooting) {
			cue->isShooting = false;
		}
	}

	// Reset the game
	if (window->KeyHold(GLFW_KEY_R)) {
		ResetGame();
	}
}

void Homework2::OnKeyPress(int key, int mods)
{
}

void Homework2::OnKeyRelease(int key, int mods)
{
}

void Homework2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// Rotate the camera around the white ball 
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		myCam->RotateThirdPerson_OY(-deltaX * sensivityOX);

		window->DisablePointer();
		window->CenterPointer();
	}
}

void Homework2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Homework2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Homework2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Homework2::OnWindowResize(int width, int height)
{
}
