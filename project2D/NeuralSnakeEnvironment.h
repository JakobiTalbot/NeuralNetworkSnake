#pragma once
#include <vector>
#include "Renderer2D.h"
class NeuralNetwork;
class NeuralSnake;
class Grid;

#define SNAKE_COUNT 20

#define INPUT_NEURON_COUNT 8
#define HIDDEN_LAYER_COUNT 1
#define HIDDEN_NEURON_COUNT 32
#define OUTPUT_NEURON_COUNT 4

#define EXPIRE_MOVES_COUNT 50

class NeuralSnakeEnvironment
{
public:
	NeuralSnakeEnvironment(Grid* pGrid);
	~NeuralSnakeEnvironment();

	void Update(float fDeltaTime);
	void Draw(aie::Renderer2D* pRenderer);

	void CreateNewGeneration();

	int CalculateFitness(NeuralSnake* snake);

private:
	NeuralNetwork* m_pNeuralNetwork;
	std::vector<NeuralSnake*> m_pSnakes;
	aie::Font* m_pFont;

	float m_fSnakeTimestep = 0.1f;
	float m_fMutationRate = 0.05f;
	bool m_bDrawStats = true;

	int m_nCurrentGeneration = 0;
	Grid* m_pGrid;

	int m_iCurrentSnake = 0;
	std::vector<int> m_nSnakeFitnesses;

	int m_fittestSnake = 0;
	unsigned int m_seed;
};