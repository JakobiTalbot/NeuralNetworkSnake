#pragma once
#include "Renderer2D.h"
#include "glm/glm.hpp"
class Grid;
class Snake;
class Pickup
{
public:
	Pickup(Grid* pGrid);
	~Pickup();

	void Update(float fDeltaTime);
	void Draw(aie::Renderer2D* pRenderer);

	void SpawnPickup(Snake* pSnake);

	glm::vec2 GetPickupNode() { return m_v2PickupNode; }

private:
	Grid* m_pGrid;
	glm::vec2 m_v2PickupNode;
};