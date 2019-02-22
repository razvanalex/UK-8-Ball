#include "Table.h"

Table::Table(float height, float displacement, float holeSize, 
	float cushionWidth, float railWidth, glm::vec3 scale, glm::vec3 position)
{
	this->height = height;
	this->displacement = displacement;
	this->holeSize = holeSize;
	this->cushionWidth = cushionWidth;
	this->railWidth = railWidth;
	this->scale = scale;
	this->position = position + glm::vec3(0, height, 0);

	glm::vec3 tablePos, legPos[4], legSize;
	glm::vec3 railPosition[4], rail1Size, rail2Size;
	glm::vec3 cushionPosition[6], cushionSizeShort, cushionSizeLong;

	// Init the positions and scales of all components of the table
	InitTable(&tablePos);
	InitLegs(&legPos, &legSize);
	InitRails(tablePos, &railPosition, &rail1Size, &rail2Size);
	InitCushions(&cushionPosition, tablePos, rail1Size, rail2Size, &cushionSizeShort, &cushionSizeLong);
	InitHoles(railPosition, cushionPosition, tablePos, rail1Size, rail2Size, cushionSizeShort, cushionSizeLong);

	// Compute the playable zone area
	playableSize = glm::vec2(scale.x, scale.z) - 2 * (railWidth + cushionWidth);
}

void Table::InitTable(glm::vec3 *tablePos)
{
	// Table
	*tablePos = position + glm::vec3(0, scale.y / 2.0f, 0);
	table = new Cube("table", *tablePos, glm::vec3(0), scale, greenColor);
}

void Table::InitLegs(glm::vec3 (*legPos)[4], glm::vec3 *legSize) 
{
	// Legs
	*legSize = glm::vec3(0.1f, height, 0.1f);
	(*legPos)[0] = position + glm::vec3(-scale.x / 2 + legSize->x / 2, -height / 2.0f, -scale.z / 2 + legSize->z / 2);
	(*legPos)[1] = position + glm::vec3(-scale.x / 2 + legSize->x / 2, -height / 2.0f, scale.z / 2 - legSize->z / 2);
	(*legPos)[2] = position + glm::vec3(scale.x / 2 - legSize->x / 2, -height / 2.0f, scale.z / 2 - legSize->z / 2);
	(*legPos)[3] = position + glm::vec3(scale.x / 2 - legSize->x / 2, -height / 2.0f, -scale.z / 2 + legSize->z / 2);

	legs[0] = new Cube("leg1", (*legPos)[0], glm::vec3(0), *legSize, woodColor);
	legs[1] = new Cube("leg2", (*legPos)[1], glm::vec3(0), *legSize, woodColor);
	legs[2] = new Cube("leg3", (*legPos)[2], glm::vec3(0), *legSize, woodColor);
	legs[3] = new Cube("leg4", (*legPos)[3], glm::vec3(0), *legSize, woodColor);
}

void Table::InitRails(glm::vec3 tablePos, glm::vec3 (*railPosition)[4], glm::vec3 *rail1Size, glm::vec3 *rail2Size)
{
	// Rails
	*rail1Size = glm::vec3(railWidth, railWidth, scale.z);
	*rail2Size = glm::vec3(scale.x - rail1Size->x, railWidth, railWidth);

	float railHeight1 = tablePos.y + scale.y / 2 + rail1Size->y / 2;
	float railHeight2 = tablePos.y + scale.y / 2 + rail2Size->y / 2;

	(*railPosition)[0] = glm::vec3(position.x - scale.x / 2 + rail1Size->x / 2, railHeight1, position.z);
	(*railPosition)[1] = glm::vec3(position.x, railHeight2, position.z - scale.z / 2 + rail2Size->z / 2);
	(*railPosition)[2] = glm::vec3(position.x + scale.x / 2- rail1Size->x / 2, railHeight1, position.z);
	(*railPosition)[3] = glm::vec3(position.x, railHeight2, position.z + scale.z / 2 - rail2Size->z / 2);

	rails[0] = new Cube("rail1", (*railPosition)[0], glm::vec3(0), *rail1Size, woodColor);
	rails[1] = new Cube("rail2", (*railPosition)[1], glm::vec3(0), *rail2Size, woodColor);
	rails[2] = new Cube("rail3", (*railPosition)[2], glm::vec3(0), *rail1Size, woodColor);
	rails[3] = new Cube("rail4", (*railPosition)[3], glm::vec3(0), *rail2Size, woodColor);
}

void Table::InitCushions(glm::vec3 (*cushionPosition)[6], glm::vec3 tablePos, 
	glm::vec3 rail1Size, glm::vec3 rail2Size, glm::vec3 *cushionSizeShort,
	glm::vec3 *cushionSizeLong)
{
	// Cushions
	*cushionSizeShort = glm::vec3(scale.x - 2 * (rail2Size.z + cushionWidth) - holeSize, 0.05f, cushionWidth);
	*cushionSizeLong = glm::vec3(cushionWidth, 0.05f, (scale.z - 2 * (rail1Size.x + cushionSizeShort->z + holeSize)) / 2.0f);

	float cushionHeightShort = tablePos.y + scale.y / 2 + cushionSizeShort->y / 2;
	float cushionHeightLong = tablePos.y + scale.y / 2 + cushionSizeLong->y / 2;

	float dLongPosX =  scale.x / 2 - rail1Size.x - cushionSizeLong->x / 2;

	float cushionShortPosX = position.x - scale.x / 2 + rail1Size.x + cushionSizeShort->x / 2 + cushionWidth + holeSize / 2;

	float dLongPosZ = scale.z / 2 - rail2Size.z - cushionSizeShort->z - cushionSizeLong->z / 2 - holeSize / 2;
	float dShortPosZ = scale.z / 2 - cushionSizeShort->z / 2 - rail1Size.x;

	(*cushionPosition)[0] = glm::vec3(cushionShortPosX, cushionHeightShort,
		position.z - dShortPosZ);

	(*cushionPosition)[1] = glm::vec3(position.x - dLongPosX, cushionHeightLong,
		position.z - dLongPosZ);

	(*cushionPosition)[2] = glm::vec3(position.x - dLongPosX, cushionHeightLong,
		position.z + dLongPosZ);

	(*cushionPosition)[3] = glm::vec3(cushionShortPosX, cushionHeightShort,
		position.z + dShortPosZ);

	(*cushionPosition)[4] = glm::vec3(position.x + dLongPosX, cushionHeightLong,
		position.z - dLongPosZ);

	(*cushionPosition)[5] = glm::vec3(position.x + dLongPosX, cushionHeightLong,
		position.z + dLongPosZ);

	cushions[0] = new Cube("cushion1", (*cushionPosition)[0], glm::vec3(0), *cushionSizeShort, darkGreenColor);
	cushions[1] = new Cube("cushion2", (*cushionPosition)[1], glm::vec3(0), *cushionSizeLong, darkGreenColor);
	cushions[2] = new Cube("cushion3", (*cushionPosition)[2], glm::vec3(0), *cushionSizeLong, darkGreenColor);
	cushions[3] = new Cube("cushion4", (*cushionPosition)[3], glm::vec3(0), *cushionSizeShort, darkGreenColor);
	cushions[4] = new Cube("cushion5", (*cushionPosition)[4], glm::vec3(0), *cushionSizeLong, darkGreenColor);
	cushions[5] = new Cube("cushion6", (*cushionPosition)[5], glm::vec3(0), *cushionSizeLong, darkGreenColor);
}

void Table::InitHoles(glm::vec3 railPosition[4], glm::vec3 cushionPosition[6], 
	glm::vec3 tablePos, glm::vec3 rail1Size, glm::vec3 rail2Size, 
	glm::vec3 cushionSizeShort, glm::vec3 cushionSizeLong)
{
	// Holes
	glm::vec3 hole1Pos = glm::vec3(
		(railPosition[0].x + cushionPosition[1].x) / 2 + displacement / 2, 
		tablePos.y + scale.y / 2, 
		(railPosition[1].z + cushionPosition[0].z) / 2 + displacement / 2);

	glm::vec3 hole2Pos = glm::vec3(
		(railPosition[0].x + cushionPosition[1].x) / 2, 
		tablePos.y + scale.y / 2,
		railPosition[0].z);

	glm::vec3 hole3Pos = glm::vec3(
		(railPosition[0].x + cushionPosition[1].x) / 2 + displacement / 2, 
		tablePos.y + scale.y / 2, 
		(railPosition[3].z + cushionPosition[3].z) / 2 - displacement / 2);

	glm::vec3 hole4Pos = glm::vec3(
		(railPosition[2].x + cushionPosition[4].x) / 2 - displacement / 2, 
		tablePos.y + scale.y / 2,
		(railPosition[3].z + cushionPosition[3].z) / 2 - displacement / 2);

	glm::vec3 hole5Pos = glm::vec3(
		(railPosition[2].x + cushionPosition[4].x) / 2, 
		tablePos.y + scale.y / 2,
		railPosition[0].z);

	glm::vec3 hole6Pos = glm::vec3(
		(railPosition[2].x + cushionPosition[4].x) / 2 - displacement / 2,
		tablePos.y + scale.y / 2,
		(railPosition[1].z + cushionPosition[0].z) / 2 + displacement / 2);

	holes[0] = new Sphere("hole1", hole1Pos, glm::vec3(0), glm::vec3(holeSize), glm::vec3(0), 10);
	holes[1] = new Sphere("hole2", hole2Pos, glm::vec3(0), glm::vec3(holeSize), glm::vec3(0), 10);
	holes[2] = new Sphere("hole3", hole3Pos, glm::vec3(0), glm::vec3(holeSize), glm::vec3(0), 10);
	holes[3] = new Sphere("hole4", hole4Pos, glm::vec3(0), glm::vec3(holeSize), glm::vec3(0), 10);
	holes[4] = new Sphere("hole5", hole5Pos, glm::vec3(0), glm::vec3(holeSize), glm::vec3(0), 10);
	holes[5] = new Sphere("hole6", hole6Pos, glm::vec3(0), glm::vec3(holeSize), glm::vec3(0), 10);
}


Table::~Table()
{
}

std::vector<Object3D*> Table::GetObjects3D()
{
	std::vector<Object3D*> result = std::vector<Object3D*>();
	
	result.push_back(table);

	for (auto l : legs)
		result.push_back(l);
	
	for (auto r : rails)
		result.push_back(r);

	for (auto c : cushions)
		result.push_back(c);
	 
	for (auto h : holes)
		result.push_back(h);

	return result;
}

std::vector<Cube*> Table::GetCollidableObjects() 
{
	std::vector<Cube*> result = std::vector<Cube*>();

	for (auto r : rails)
		result.push_back(r);

	for (auto c : cushions)
		result.push_back(c);

	return result;
}

std::vector<Sphere*> Table::GetHoles()
{
	std::vector<Sphere*> result = std::vector<Sphere*>();

	for (auto h : holes)
		result.push_back(h);

	return result;
}


glm::vec2 Table::GetPlayableSize()
{
	return this->playableSize;
}

glm::vec3 Table::GetPosition()
{
	return this->position;
}
