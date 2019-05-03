#pragma once
#include "glm/glm.hpp"
#include "Renderer2D.h"
#define GRID_WIDTH 10
#define GRID_HEIGHT 10
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

	void Init(int nWindowHeight, int nWindowWidth);

	void Draw(aie::Renderer2D* pRenderer);

	Node** GetNodes() { return m_pNodes; }
private:
	// stores the grid's nodes
	Node** m_pNodes;
};