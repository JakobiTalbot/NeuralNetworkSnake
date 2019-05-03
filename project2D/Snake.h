#pragma once
#include <vector>
#include "Renderer2D.h"
#include <glm/glm.hpp>
class Grid;
class Pickup;
class Application2D;
struct Node;
enum class eDirection
{
	Up = 0,
	Right,
	Down,
	Left
};
class Snake
{
public:
	Snake(Grid* pGrid);
	~Snake();
	
	void Update(float fDeltaTime, Pickup* pPickup);
	void Draw(aie::Renderer2D* pRenderer);

	std::vector<Node*> GetSnakeNodes() { return m_pSnakeNodes; }

private:
	Grid* m_pGrid;
	std::vector<Node*> m_pSnakeNodes;
	eDirection m_nextDirection;
	eDirection m_lastDirection;

	glm::vec2 m_v2HeadNode;

	float m_fTimeToNextMove;
	const float m_fTimeBetweenMovements = 0.5f;

	bool m_bIncreasingSize = false;
};