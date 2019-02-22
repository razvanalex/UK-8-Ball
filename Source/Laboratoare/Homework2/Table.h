#pragma once

#include <include/glm.h>
#include <vector>
#include "Object3D.h"
#include "Cube.h"
#include "Sphere.h"

class Table {
	public:
		Table(float height, float displacement, float holeSize, 
			float cushionWidth, float railWidth, glm::vec3 scale, 
			glm::vec3 position);
		~Table();

	public:
		std::vector<Object3D*> GetObjects3D();
		std::vector<Cube*> GetCollidableObjects();
		std::vector<Sphere*> Table::GetHoles();
		glm::vec2 GetPlayableSize();
		glm::vec3 GetPosition();

	private:
		// Table components
		Cube *table;
		Cube *legs[4];
		Cube *rails[4];
		Cube *cushions[6];
		Sphere *holes[6];
		glm::vec2 playableSize;

		float height = .75f;
		float displacement = 0.05f;
		float holeSize = 0.1f;
		float cushionWidth = 0.05f;
		float railWidth = 0.05f;

		glm::vec3 scale = glm::vec3(1, 0.2f, 2);
		glm::vec3 position = glm::vec3(0, height, 0);

		const glm::vec3 woodColor = glm::vec3(0.5, 0.3, 0);
		const glm::vec3 greenColor = glm::vec3(0, 1, 0);
		const glm::vec3 darkGreenColor = glm::vec3(0, 0.8f, 0);


	private:
		void InitTable(glm::vec3 * tablePos);
		void InitLegs(glm::vec3(*legPos)[4], glm::vec3 * legSize);
		void InitRails(glm::vec3 tablePos, glm::vec3 (*railPosition)[4], glm::vec3 * rail1Size, glm::vec3 * rail2Size);
		void InitCushions(glm::vec3 (*cushionPosition)[6], glm::vec3 tablePos, glm::vec3 rail1Size, glm::vec3 rail2Size, glm::vec3 * cushionSizeShort, glm::vec3 * cushionSizeLong);
		void InitHoles(glm::vec3 railPosition[4], glm::vec3 cushionPosition[6], glm::vec3 tablePos, glm::vec3 rail1Size, glm::vec3 rail2Size, glm::vec3 cushionSizeShort, glm::vec3 cushionSizeLong);
};
