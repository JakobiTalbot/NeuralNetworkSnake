#pragma once
#include <vector>
#include "Renderer2D.h"
class NeuralNetwork;
class NeuralSnake;
class Grid;

#define SNAKE_COUNT 20
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

	float m_fSnakeTimestep = 0.2f;

	int m_nCurrentGeneration = 0;
	Grid* m_pGrid;

	int m_iCurrentSnake = 0;
	std::vector<int> m_nSnakeFitnesses;

	unsigned int m_seed;
};