#include "NeuralSnake.h"
#include "Grid.h"
#include "Input.h"
#include "Pickup.h"
#include "Application2D.h"
#include "NeuralSnakeEnvironment.h"
#include "NeuralNetwork.h"
#include <math.h>

NeuralSnake::NeuralSnake(Grid* pGrid, NeuralNetwork* pNeuralNetwork, bool bWrapAround, float fMutationRate, float fTimestep)
{
	m_pGrid = pGrid;
	m_bWrapAround = bWrapAround;
	m_fTimeToNextMove = fTimestep;
	m_fTimestep = fTimestep;
	m_nextDirection = eNeuralDirection::Up;
	m_lastDirection = eNeuralDirection::Up;
	// start in centre
	m_v2HeadNode = { GRID_WIDTH / 2, GRID_HEIGHT / 2 };
	m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][(int)m_v2HeadNode.y]);
	// copy master neural network
	m_pNeuralNetwork = new NeuralNetwork(*pNeuralNetwork);
	if (fMutationRate > 0.005f)
		m_pNeuralNetwork->Mutate(fMutationRate);
}

NeuralSnake::~NeuralSnake()
{
	delete m_pNeuralNetwork;
}

bool NeuralSnake::Update(float fDeltaTime)
{
	m_fTimeToNextMove -= fDeltaTime;
	// move
	while (m_fTimeToNextMove <= 0.f)
	{
		// add time to timer
		m_fTimeToNextMove += m_fTimestep;
		// get pickup instance
		Pickup* pPickup = Pickup::GetInstance();
		// get distance to food in x and y coordinates
		glm::vec2 v2DistanceToFood = pPickup->GetPickupNode() - m_v2HeadNode;
		// U-R-D-L (-1/1)
		// 0-3: is obstacle directly next to us in this direction
		// 4-7: is food in this direction
		float fInput[INPUT_NEURON_COUNT];
		for (int i = 0; i < INPUT_NEURON_COUNT; ++i)
		{
			i < 4 ? fInput[i] = 1 : fInput[i] = -1;
		}

		if (m_v2HeadNode.y >= GRID_HEIGHT - 1.5f) // if at top of grid
			fInput[0] = -1;
		else if (m_v2HeadNode.y < 0.5f) // if at bottom of grid
			fInput[2] = -1;
		if (m_v2HeadNode.x >= GRID_WIDTH - 1.5f) // if at right of grid
			fInput[1] = -1;
		else if (m_v2HeadNode.x < 0.5f) // if at left of grid
			fInput[3] = -1;

		if (fInput[0] < 0.5f)
		{
			// if node above contains snake
			std::vector<Node*>::iterator nodeIterator = std::find(m_pSnakeNodes.begin() + 1, m_pSnakeNodes.end(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][(int)m_v2HeadNode.y + 1]);
			if (nodeIterator != m_pSnakeNodes.end()
				&& nodeIterator != m_pSnakeNodes.end() - 1)
				fInput[0] = -1;
		}
		if (fInput[1] < 0.5f)
		{
			// if node to right contains snake
			std::vector<Node*>::iterator nodeIterator = std::find(m_pSnakeNodes.begin() + 1, m_pSnakeNodes.end(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x + 1][(int)m_v2HeadNode.y]);
			if (nodeIterator != m_pSnakeNodes.end()
				&& nodeIterator != m_pSnakeNodes.end() - 1)
				fInput[0] = -1;
		}
		if (fInput[2] < 0.5f)
		{
			// if node down contains snake
			std::vector<Node*>::iterator nodeIterator = std::find(m_pSnakeNodes.begin() + 1, m_pSnakeNodes.end(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][(int)m_v2HeadNode.y - 1]);
			if (nodeIterator != m_pSnakeNodes.end()
				&& nodeIterator != m_pSnakeNodes.end() - 1)
				fInput[0] = -1;
		}
		if (fInput[3] < 0.5f)
		{
			// if node to left contains snake
			std::vector<Node*>::iterator nodeIterator = std::find(m_pSnakeNodes.begin() + 1, m_pSnakeNodes.end(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x - 1][(int)m_v2HeadNode.y]);
			if (nodeIterator != m_pSnakeNodes.end()
				&& nodeIterator != m_pSnakeNodes.end() - 1)
				fInput[0] = -1;
		}

		if (v2DistanceToFood.x < v2DistanceToFood.y)
		{
			// if food to left
			if (v2DistanceToFood.x < 0.f)
				fInput[7] = 1;
			else
				fInput[5] = 1;
		}
		else
		{
			// if food down
			if (v2DistanceToFood.y < 0.f)
				fInput[6] = 1;
			else
				fInput[4] = 1;
		}

		//// if on same x
		//if (v2DistanceToFood.x < 0.5f && v2DistanceToFood.x > -0.5f)
		//	v2DistanceToFood.y > 0.f ? fInput[4] = 1 : fInput[6] = 1;
		//// if on same y
		//if (v2DistanceToFood.y < 0.5f && v2DistanceToFood.y > -0.5f)
		//	v2DistanceToFood.x > 0.f ? fInput[5] = 1 : fInput[7] = 1;

		//v2DistanceToFood.x > v2DistanceToFood.y ? v2DistanceToFood.x > 0.f ? fInput[5] = 1 : fInput[7] = 1 : v2DistanceToFood.y > 0.f ? fInput[4] = 1 : fInput[6] = 1;

		// create array to store output
		float fOutput[OUTPUT_NEURON_COUNT];
		// get output
		m_pNeuralNetwork->GetOutput(fInput, fOutput);

		float fBestOutput = 0.f;
		for (int i = 0; i < OUTPUT_NEURON_COUNT; ++i)
		{
			if (fOutput[i] > fBestOutput)
			{
				if (m_lastDirection == eNeuralDirection::Up
					&& (eNeuralDirection)i == eNeuralDirection::Down
					|| m_lastDirection == eNeuralDirection::Right
					&& (eNeuralDirection)i == eNeuralDirection::Left
					|| m_lastDirection == eNeuralDirection::Left
					&& (eNeuralDirection)i == eNeuralDirection::Right
					|| m_lastDirection == eNeuralDirection::Down
					&& (eNeuralDirection)i == eNeuralDirection::Up)
					continue;

				fBestOutput = fOutput[i];
				m_nextDirection = (eNeuralDirection)i;
			}
		}

		// move in direction
		switch (m_nextDirection)
		{
		case eNeuralDirection::Up:
			if (m_v2HeadNode.y < GRID_HEIGHT - 1)
			{
				m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][(int)m_v2HeadNode.y + 1]);
				m_v2HeadNode.y++;
			}
			else
			{
				// check if we can wrap around the borders
				if (m_bWrapAround)
				{
					// wrap around the border
					m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][0]);
					m_v2HeadNode.y = 0;
				}
				else
				{
					return false;
				}
			}
			break;

		case eNeuralDirection::Right:
			if (m_v2HeadNode.x < GRID_WIDTH - 1)
			{
				m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x + 1][(int)m_v2HeadNode.y]);
				m_v2HeadNode.x++;
			}
			else
			{
				// check if we can wrap around the borders
				if (m_bWrapAround)
				{
					// wrap around the border
					m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[0][(int)m_v2HeadNode.y]);
					m_v2HeadNode.x = 0;
				}
				else
				{
					return false;
				}
			}
			break;

		case eNeuralDirection::Down:
			if (m_v2HeadNode.y != 0)
			{
				m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][(int)m_v2HeadNode.y - 1]);
				m_v2HeadNode.y--;
			}
			else
			{
				// check if we can wrap around the borders
				if (m_bWrapAround)
				{
					// wrap around the border
					m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][GRID_HEIGHT - 1]);
					m_v2HeadNode.y = GRID_HEIGHT - 1;
				}
				else
				{
					return false;
				}
			}
			break;

		case eNeuralDirection::Left:
			if (m_v2HeadNode.x != 0)
			{
				m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x - 1][(int)m_v2HeadNode.y]);
				m_v2HeadNode.x--;
			}
			else
			{
				// check if we can wrap around the borders
				if (m_bWrapAround)
				{
					// wrap around the border
					m_pSnakeNodes.insert(m_pSnakeNodes.begin(), &m_pGrid->GetNodes()[GRID_WIDTH - 1][(int)m_v2HeadNode.y]);
					m_v2HeadNode.x = GRID_WIDTH - 1;
				}
				else
				{
					return false;
				}
			}
			break;
		}

		// find if snake collides with itself
		std::vector<Node*>::iterator nodeIterator = std::find(m_pSnakeNodes.begin() + 1, m_pSnakeNodes.end(), &m_pGrid->GetNodes()[(int)m_v2HeadNode.x][(int)m_v2HeadNode.y]);
		if (nodeIterator != m_pSnakeNodes.end()
			&& nodeIterator != m_pSnakeNodes.end() - 1)
		{
			// return false if colliding
			return false;
		}

		m_lastDirection = m_nextDirection;
		m_nMovesSinceLastFood++;
		// check if we entered the node with the pickup in it
		if (m_v2HeadNode == pPickup->GetPickupNode())
		{
			// spawn new pickup
			pPickup->MovePickupNeural(this);
			m_nSize++;
			m_nMovesSinceLastFood = 0;
			// don't decrease in size (effectively adds one to size)
			m_bIncreasingSize = true;
		}

		if (m_nMovesSinceLastFood >= EXPIRE_MOVES_COUNT)
			return false;

		// remove last tile if not increasing size
		if (!m_bIncreasingSize)
			m_pSnakeNodes.pop_back();
		else
			m_bIncreasingSize = false;

		m_nMoves++;

		system("cls");
		// debug print
		for (int i = 0; i < INPUT_NEURON_COUNT; ++i)
		{
			if (i < OUTPUT_NEURON_COUNT)
				printf("INPUT %i: %.3f\tOUTPUT %i: %.3f\n", i, fInput[i], i, fOutput[i]);
			else
				printf("INPUT %i: %.3f\n", i, fInput[i]);
		}
	}

	// return true if still alive
	return true;
}

void NeuralSnake::Draw(aie::Renderer2D* pRenderer)
{
	// prevent errors if snake has died at size of 1 node
	if (m_pSnakeNodes.size() < 1)
		return;

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
	case (eNeuralDirection::Up):
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
	case (eNeuralDirection::Right):
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
	case (eNeuralDirection::Down):
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
	case (eNeuralDirection::Left):
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