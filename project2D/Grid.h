#pragma once
#include "glm/glm.hpp"
#include "Renderer2D.h"
// defines the size of the grid to be created
#define GRID_WIDTH 10
#define GRID_HEIGHT 10
// stores values for a node
struct Node
{
	glm::vec2 m_v2Position;
	glm::vec2 m_v2Extents;
};

class Grid
{
public:
	Grid();
	~Grid();

	/*	@brief Initialises the array of nodes in a grid
	*/
	void Init();

	/*	@brief Draws the array of nodes to the screen each frame
		@param Renderer2D to draw the nodes to
	*/
	void Draw(aie::Renderer2D* pRenderer);

	/*	@brief Gets the 2d array of nodes in the grid
		@return The nodes stored in the grid
	*/
	Node** GetNodes() { return m_pNodes; }

private:
	// stores the grid's nodes (2d array)
	Node** m_pNodes;
};