#include "Pickup.h"
#include "Grid.h"
#include "Snake.h"
#include "Application2D.h"
#include <time.h>
Pickup::Pickup(Grid* pGrid)
{
	m_pGrid = pGrid;
	// seed random
	srand((unsigned int)time(NULL));
}

Pickup::~Pickup()
{
}

void Pickup::Draw(aie::Renderer2D* pRenderer)
{
	// get reference to node with pickup
	Node* pNode = &m_pGrid->GetNodes()[(int)m_v2PickupNode.x][(int)m_v2PickupNode.y];
	// set colour
	pRenderer->setRenderColour(1, 0, 0);
	pRenderer->drawBox(pNode->m_v2Position.x, pNode->m_v2Position.y,
		pNode->m_v2Extents.x, pNode->m_v2Extents.y);
}

void Pickup::MovePickup(Snake* pSnake)
{
	bool bFoundSpot = false;
	std::vector<Node*> pSnakeNodes = pSnake->GetSnakeNodes();
	std::vector<Node*>::iterator nodeIterator;
	while (!bFoundSpot)
	{
		// check if won
		if (pSnakeNodes.size() == GRID_WIDTH * GRID_HEIGHT)
		{
			// quit if won
			Application2D::GetInstance()->quit();
		}
		// spawn pickup at random location
		m_v2PickupNode = { rand() % GRID_WIDTH, rand() % GRID_HEIGHT };
		nodeIterator = std::find(pSnakeNodes.begin(), pSnakeNodes.end(), &m_pGrid->GetNodes()[(int)m_v2PickupNode.x][(int)m_v2PickupNode.y]);
		if (nodeIterator == pSnakeNodes.end())
			bFoundSpot = true;
	}
}

void Pickup::MovePickupNeural(NeuralSnake * pSnake)
{
}
