#include "NeuralSnakeEnvironment.h"
#include "NeuralNetwork.h"
#include "NeuralSnake.h"
#include "Application2D.h"
#include "Grid.h"
#include "Pickup.h"
#include "Input.h"
#include "Font.h"
#include <random>
#include <time.h>


NeuralSnakeEnvironment::NeuralSnakeEnvironment(Grid* pGrid)
{
	m_pFont = new aie::Font("./font/consolas.ttf", 16);
	// initialise neural network
	m_pNeuralNetwork = new NeuralNetwork(INPUT_NEURON_COUNT, HIDDEN_LAYER_COUNT, HIDDEN_NEURON_COUNT, OUTPUT_NEURON_COUNT);
	// create snakes
	for (int i = 0; i < SNAKE_COUNT; ++i)
		m_pSnakes.push_back(new NeuralSnake(pGrid, m_pNeuralNetwork, false, 0.5f, m_fSnakeTimestep));

	m_seed = (unsigned int)time(NULL);
	m_pSnakes[0]->SeedRandom(m_seed);
	m_pGrid = pGrid;
}

NeuralSnakeEnvironment::~NeuralSnakeEnvironment()
{
	delete m_pFont;
	delete m_pNeuralNetwork;
	for (auto& snake : m_pSnakes)
	{
		delete snake;
	}
}

void NeuralSnakeEnvironment::Update(float fDeltaTime)
{
	if (aie::Input::getInstance()->wasKeyPressed(aie::INPUT_KEY_UP))
	{
		m_fSnakeTimestep += 0.01f;
		for (auto& snake : m_pSnakes)
			snake->SetTimestep(m_fSnakeTimestep);
	}
	else if (aie::Input::getInstance()->wasKeyPressed(aie::INPUT_KEY_DOWN))
	{
		m_fSnakeTimestep -= 0.01f;
		for (auto& snake : m_pSnakes)
			snake->SetTimestep(m_fSnakeTimestep);
	}

	// update current snake, check if died
	if (!m_pSnakes[m_iCurrentSnake]->Update(fDeltaTime))
	{
		// get fitness
		m_nSnakeFitnesses[m_iCurrentSnake] = ((m_pSnakes[m_iCurrentSnake]->GetSize() - 1) * 50.f) + m_pSnakes[m_iCurrentSnake]->GetMoves();
		// go to next snake if current one dies
		m_iCurrentSnake++;
		// create new generation if through each snake
		if (m_iCurrentSnake == SNAKE_COUNT)
		{
			CreateNewGeneration();
		}
		m_pSnakes[m_iCurrentSnake]->SeedRandom(m_seed);
		Pickup::GetInstance()->MovePickupNeural(m_pSnakes[m_iCurrentSnake]);
	}
}

void NeuralSnakeEnvironment::Draw(aie::Renderer2D* pRenderer)
{
	// draw current snake
	if (m_pSnakes[m_iCurrentSnake])
		m_pSnakes[m_iCurrentSnake]->Draw(pRenderer);
	// draw stats
	char gen[32];
	char snakenum[32];
	char timestep[32];
	sprintf_s(gen, 32, "GEN: %i", m_nCurrentGeneration);
	sprintf_s(snakenum, 32, "NO. %i", m_iCurrentSnake);
	sprintf_s(timestep, 32, "TIMESTEP: %.2f", m_fSnakeTimestep);
	pRenderer->setRenderColour(1, 1, 1);
	pRenderer->drawText(m_pFont, gen, 4, Application2D::GetInstance()->getWindowHeight() - 16);
	pRenderer->drawText(m_pFont, snakenum, 4, Application2D::GetInstance()->getWindowHeight() - 32);
	pRenderer->drawText(m_pFont, timestep, 4, Application2D::GetInstance()->getWindowHeight() - 48);
}

void NeuralSnakeEnvironment::CreateNewGeneration()
{
	delete m_pNeuralNetwork;
	// get new neural network base
	m_pNeuralNetwork = new NeuralNetwork(*m_pSnakes[m_pNeuralNetwork->RouletteSelect(m_nSnakeFitnesses, SNAKE_COUNT)]->GetNeuralNetwork());
	// reset current snake index
	m_iCurrentSnake = 0;
	for (auto& snake : m_pSnakes)
	{
		delete snake;
	}
	m_pSnakes.clear();
	m_seed = (unsigned int)time(NULL);
	// create snakes
	for (int i = 0; i < SNAKE_COUNT; ++i)
		m_pSnakes.push_back(new NeuralSnake(m_pGrid, m_pNeuralNetwork, false, 0.1f, m_fSnakeTimestep));
	m_pSnakes[0]->SeedRandom(m_seed);
	m_nCurrentGeneration++;
}