#pragma once
#include "Renderer2D.h"
#include "glm/glm.hpp"
class Grid;
class Snake;
class NeuralSnake;
class Pickup
{
public:
	Pickup(Grid* pGrid);
	~Pickup();

	/*	@brief Draws the pickup to the screen each frame
		@param The Renderer2D to draw the pickup to
	*/
	void Draw(aie::Renderer2D* pRenderer);

	/*	@brief Moves the pickup to a random coordinate on the grid,
		 avoids placing the pickup node on a node with the snake in it
		@param The snake to avoid placing the pickup node on
	*/
	void MovePickup(Snake* pSnake);

	void MovePickupNeural(NeuralSnake* pSnake);

	/*	@brief Gets the coordinates of the node where the pickup is currently located
		@return The coordinates of the node where the pickup is located
	*/
	glm::vec2 GetPickupNode() { return m_v2PickupNode; }

	static Pickup* GetInstance() { return m_pInstance; }

private:
	// stores a reference to the grid
	Grid* m_pGrid;
	// stores the coordinates of the node where the pickup is located on the grid
	glm::vec2 m_v2PickupNode;

	static Pickup* m_pInstance;
};