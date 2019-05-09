#include "Snake.h"
#include "Grid.h"
#include "Input.h"
#include "Pickup.h"
#include "Application2D.h"
Snake::Snake(Grid* pGrid)
{
	m_pGrid = pGrid;
	m_fTimeToNextMove = TIME_BETWEEN_MOVEMENTS;
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
	if (pInput->isKeyDown(aie::INPUT_KEY_UP)
		&& m_lastDirection != eDirection::Down)
		m_nextDirection = eDirection::Up;
	else if (pInput->isKeyDown(aie::INPUT_KEY_RIGHT)
		&& m_lastDirection != eDirection::Left)
		m_nextDirection = eDirection::Right;
	else if (pInput->isKeyDown(aie::INPUT_KEY_DOWN)
		&& m_lastDirection != eDirection::Up)
		m_nextDirection = eDirection::Down;
	else if (pInput->isKeyDown(aie::INPUT_KEY_LEFT)
		&& m_lastDirection != eDirection::Right)
		m_nextDirection = eDirection::Left;

	m_fTimeToNextMove -= fDeltaTime;
	// move
	while (m_fTimeToNextMove <= 0.f)
	{
		// add time to timer
		m_fTimeToNextMove += TIME_BETWEEN_MOVEMENTS;

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
		
		// find if snake collides with itself
		std::vector<Node*>::iterator nodeIterator = std::find(m_pSnakeNodes.begin() + 1, m_pSnakeNodes.end(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][(int)m_v2HeadNode.y]);
		if (nodeIterator != m_pSnakeNodes.end()
			&& nodeIterator != m_pSnakeNodes.end() - 1)
		{
			// quit if colliding
			Application2D::GetInstance()->quit();
		}

		m_lastDirection = m_nextDirection;

		// check if we entered the node with the pickup in it
		if (m_v2HeadNode == pPickup->GetPickupNode())
		{
			// spawn new pickup
			pPickup->MovePickup(this);
			m_nSize++;
			// don't decrease in size (effectively adds one to size)
			m_bIncreasingSize = true;
		}

		// remove last tile if not increasing size
		if (!m_bIncreasingSize)
			m_pSnakeNodes.pop_back();
		else
			m_bIncreasingSize = false;
	}
}

void Snake::Draw(aie::Renderer2D* pRenderer)
{
	pRenderer->setRenderColour(0, 0.8f, 0);
	// draw snake nodes
	for (auto& node : m_pSnakeNodes)
	{
		
		pRenderer->drawBox(node->m_v2Position.x, node->m_v2Position.y,
			node->m_v2Extents.x, node->m_v2Extents.y, 0.f, 10.f);
	}

	// get all possible eye positions
	glm::vec2 v2TopRightEyePos = { m_pSnakeNodes[0]->m_v2Position.x + m_pSnakeNodes[0]->m_v2Extents.x * 0.25f,
				m_pSnakeNodes[0]->m_v2Position.y + m_pSnakeNodes[0]->m_v2Extents.y * 0.25f };
	glm::vec2 v2BottomLeftEyePos = { m_pSnakeNodes[0]->m_v2Position.x - m_pSnakeNodes[0]->m_v2Extents.x * 0.25f,
				m_pSnakeNodes[0]->m_v2Position.y - m_pSnakeNodes[0]->m_v2Extents.y * 0.25f };

	pRenderer->setRenderColour(1, 1, 1);
	// draw eyes
	switch (m_lastDirection)
	{
		case (eDirection::Up):
			// white part of eyes
			// top left
			pRenderer->drawCircle(v2BottomLeftEyePos.x, v2TopRightEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_WHITE_SCALE, 10.f);
			// top right
			pRenderer->drawCircle(v2TopRightEyePos.x, v2TopRightEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_WHITE_SCALE, 10.f);

			// black part of eyes
			pRenderer->setRenderColour(0, 0, 0);
			// top left
			pRenderer->drawCircle(v2BottomLeftEyePos.x, v2TopRightEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_BLACK_SCALE, 10.f);
			// top right
			pRenderer->drawCircle(v2TopRightEyePos.x, v2TopRightEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_BLACK_SCALE, 10.f);
			break;
		case (eDirection::Right):
			// white part of eyes
			// top right
			pRenderer->drawCircle(v2TopRightEyePos.x, v2TopRightEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_WHITE_SCALE, 10.f);
			// bottom right
			pRenderer->drawCircle(v2TopRightEyePos.x, v2BottomLeftEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_WHITE_SCALE, 10.f);

			// black part of eyes
			pRenderer->setRenderColour(0, 0, 0);
			// top right
			pRenderer->drawCircle(v2TopRightEyePos.x, v2TopRightEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_BLACK_SCALE, 10.f);
			// bottom right
			pRenderer->drawCircle(v2TopRightEyePos.x, v2BottomLeftEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_BLACK_SCALE, 10.f);
			break;
		case (eDirection::Down):
			// white part of eyes
			// bottom left
			pRenderer->drawCircle(v2BottomLeftEyePos.x, v2BottomLeftEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_WHITE_SCALE, 10.f);
			// bottom right
			pRenderer->drawCircle(v2TopRightEyePos.x, v2BottomLeftEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_WHITE_SCALE, 10.f);

			// black part of eyes
			pRenderer->setRenderColour(0, 0, 0);
			// bottom left
			pRenderer->drawCircle(v2BottomLeftEyePos.x, v2BottomLeftEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_BLACK_SCALE, 10.f);
			// bottom right
			pRenderer->drawCircle(v2TopRightEyePos.x, v2BottomLeftEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_BLACK_SCALE, 10.f);
			break;
		case (eDirection::Left):
			// white part of eyes
			// bottom left
			pRenderer->drawCircle(v2BottomLeftEyePos.x, v2BottomLeftEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_WHITE_SCALE, 10.f);
			// top left
			pRenderer->drawCircle(v2BottomLeftEyePos.x, v2TopRightEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_WHITE_SCALE, 10.f);

			// black part of eyes
			pRenderer->setRenderColour(0, 0, 0);
			// bottom left
			pRenderer->drawCircle(v2BottomLeftEyePos.x, v2BottomLeftEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_BLACK_SCALE, 10.f);
			// top left
			pRenderer->drawCircle(v2BottomLeftEyePos.x, v2TopRightEyePos.y,
				((m_pSnakeNodes[0]->m_v2Extents.x + m_pSnakeNodes[0]->m_v2Extents.y) * 0.5f) * EYE_BLACK_SCALE, 10.f);
			break;
	}
}