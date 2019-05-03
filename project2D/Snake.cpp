#include "Snake.h"
#include "Grid.h"
#include "Input.h"
#include "Pickup.h"
#include "Application2D.h"
Snake::Snake(Grid* pGrid)
{
	m_pGrid = pGrid;
	
	m_fTimeToNextMove = m_fTimeBetweenMovements;
	m_nextDirection = eDirection::Up;
	m_lastDirection = eDirection::Up;
	// start in centre
	m_v2HeadNode = { GRID_WIDTH / 2, GRID_HEIGHT / 2 };
	m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][(int)m_v2HeadNode.y]);
}

Snake::~Snake()
{
}

void Snake::Update(float fDeltaTime, Pickup* pPickup)
{
	aie::Input* pInput = aie::Input::getInstance();

	// get input direction
	if (pInput->wasKeyPressed(aie::INPUT_KEY_UP)
		&& m_lastDirection != eDirection::Down)
		m_nextDirection = eDirection::Up;
	else if (pInput->wasKeyPressed(aie::INPUT_KEY_RIGHT)
		&& m_lastDirection != eDirection::Left)
		m_nextDirection = eDirection::Right;
	else if (pInput->wasKeyPressed(aie::INPUT_KEY_DOWN)
		&& m_lastDirection != eDirection::Up)
		m_nextDirection = eDirection::Down;
	else if (pInput->wasKeyPressed(aie::INPUT_KEY_LEFT)
		&& m_lastDirection != eDirection::Right)
		m_nextDirection = eDirection::Left;

	m_fTimeToNextMove -= fDeltaTime;
	// move
	while (m_fTimeToNextMove <= 0.f)
	{
		// add time to timer
		m_fTimeToNextMove += m_fTimeBetweenMovements;

		// move in direction
		switch (m_nextDirection)
		{
			case eDirection::Up:
				if (m_v2HeadNode.y < GRID_HEIGHT - 1)
				{
					m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][(int)m_v2HeadNode.y + 1]);
					m_v2HeadNode.y++;
				}
				else
				{
					m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][0]);
					m_v2HeadNode.y = 0;
				}
				break;

			case eDirection::Right:
				if (m_v2HeadNode.x < GRID_WIDTH - 1)
				{
					m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x + 1][(int)m_v2HeadNode.y]);
					m_v2HeadNode.x++;
				}
				else
				{
					m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[0][(int)m_v2HeadNode.y]);
					m_v2HeadNode.x = 0;
				}
				break;

			case eDirection::Down:
				if (m_v2HeadNode.y != 0)
				{
					m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][(int)m_v2HeadNode.y - 1]);
					m_v2HeadNode.y--;
				}
				else
				{
					m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][GRID_HEIGHT - 1]);
					m_v2HeadNode.y = GRID_HEIGHT - 1;
				}
				break;

			case eDirection::Left:
				if (m_v2HeadNode.x != 0)
				{
					m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x - 1][(int)m_v2HeadNode.y]);
					m_v2HeadNode.x--;
				}
				else
				{
					m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[GRID_WIDTH - 1][(int)m_v2HeadNode.y]);
					m_v2HeadNode.x = GRID_WIDTH - 1;
				}
				break;
		}

		// check if we entered the node with the pickup in it
		if (m_v2HeadNode == pPickup->GetPickupNode())
		{
			// spawn new pickup
			pPickup->SpawnPickup(this);
			// don't decrease in size (effectively adds one to size)
			m_bIncreasingSize = true;
		}
		
		// find if snake collides with itself
		std::vector<Node*>::iterator nodeIterator = std::find(m_pSnakeNodes.begin() + 1, m_pSnakeNodes.end(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][(int)m_v2HeadNode.y]);
		if (nodeIterator != m_pSnakeNodes.end())
		{
			// quit if colliding
			Application2D::GetInstance()->quit();
		}

		m_lastDirection = m_nextDirection;

		// remove last tile if not increasing size
		if (!m_bIncreasingSize)
			m_pSnakeNodes.pop_back();
		else
			m_bIncreasingSize = false;
	}
}

void Snake::Draw(aie::Renderer2D* pRenderer)
{
	// draw snake nodes
	for (auto& node : m_pSnakeNodes)
	{
		pRenderer->setRenderColour(0, 0.8, 0);
		pRenderer->drawBox(node->m_v2Position.x, node->m_v2Position.y,
			node->m_v2Extents.x, node->m_v2Extents.y);
	}
}