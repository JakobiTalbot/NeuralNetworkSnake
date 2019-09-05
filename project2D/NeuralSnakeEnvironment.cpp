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
	// create snakes all with completely random weights/biases
	for (int i = 0; i < SNAKE_COUNT; ++i)
		m_pSnakes.push_back(new NeuralSnake(pGrid, m_pNeuralNetwork, false, 1.f, m_fSnakeTimestep));

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
	aie::Input* input = aie::Input::getInstance();
	if (input->wasKeyPressed(aie::INPUT_KEY_UP))
	{
		m_fSnakeTimestep += 0.01f;
		for (auto& snake : m_pSnakes)
			snake->SetTimestep(m_fSnakeTimestep);
	}
	else if (input->wasKeyPressed(aie::INPUT_KEY_DOWN))
	{
		m_fSnakeTimestep -= 0.01f;
		for (auto& snake : m_pSnakes)
			snake->SetTimestep(m_fSnakeTimestep);
	}

	// alter mutation rate
	if (input->wasKeyPressed(aie::INPUT_KEY_RIGHT))
		m_fMutationRate += 0.01f;
	else if (input->wasKeyPressed(aie::INPUT_KEY_LEFT))
		m_fMutationRate -= 0.01f;

	if (input->wasKeyPressed(aie::INPUT_KEY_TAB))
		m_bDrawStats = !m_bDrawStats;

	// update current snake, check if died
	if (!m_pSnakes[m_iCurrentSnake]->Update(fDeltaTime))
	{
		// get fitness
		m_nSnakeFitnesses.push_back(CalculateFitness(m_pSnakes[m_iCurrentSnake]));
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

	if (!m_bDrawStats)
		return;

	// draw stats
	char gen[16];
	char snakenum[16];
	char timestep[16];
	char fitness[16];
	char mutation[16];
	sprintf_s(gen, 16, "GEN: %i", m_nCurrentGeneration);
	sprintf_s(snakenum, 16, "NO. %i", m_iCurrentSnake);
	sprintf_s(timestep, 16, "TIMESTEP: %.2f", m_fSnakeTimestep);
	sprintf_s(fitness, 16, "FITNESS: %i", CalculateFitness(m_pSnakes[m_iCurrentSnake]));
	sprintf_s(mutation, 16, "MUTATION: %.2f", m_fMutationRate);
	pRenderer->setRenderColour(1, 1, 1);
	unsigned int height = Application2D::GetInstance()->getWindowHeight();
	pRenderer->drawText(m_pFont, gen, 4, height - 16);
	pRenderer->drawText(m_pFont, snakenum, 4, height - 32);
	pRenderer->drawText(m_pFont, timestep, 4, height - 48);
	pRenderer->drawText(m_pFont, fitness, 4, height - 64);
	pRenderer->drawText(m_pFont, mutation, 4, height - 80);
}

void NeuralSnakeEnvironment::CreateNewGeneration()
{
	// get new neural network base
	int i = m_pNeuralNetwork->RouletteSelect(m_nSnakeFitnesses);
	delete m_pNeuralNetwork;
	m_pNeuralNetwork = new NeuralNetwork(*m_pSnakes[i]->GetNeuralNetwork());

	// reset current snake index
	m_iCurrentSnake = 0;
	for (auto& snake : m_pSnakes)
		delete snake;
	m_pSnakes.clear();

	// create snakes
	for (int i = 0; i < SNAKE_COUNT; ++i)
		m_pSnakes.push_back(new NeuralSnake(m_pGrid, m_pNeuralNetwork, false, m_fMutationRate, m_fSnakeTimestep));
	m_seed = (unsigned int)time(NULL);
	m_pSnakes[0]->SeedRandom(m_seed);
	m_nCurrentGeneration++;
	m_nSnakeFitnesses.clear();
}

int NeuralSnakeEnvironment::CalculateFitness(NeuralSnake* snake)
{
	return ((snake->GetSize() - 1) * 500.f) + snake->GetMoves();
}