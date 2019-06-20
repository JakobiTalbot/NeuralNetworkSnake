#pragma once
#include <vector>
#include "Renderer2D.h"
#include "Grid.h"
#include <glm/glm.hpp>
class Grid;
class Application2D;
class NeuralNetwork;
struct Node;

#define INPUT_NEURON_COUNT 6
#define HIDDEN_LAYER_COUNT 2
#define HIDDEN_NEURON_COUNT 8
#define OUTPUT_NEURON_COUNT 4
// the directions in which the snake can move in
enum class eNeuralDirection
{
	Up = 0,
	Right,
	Down,
	Left
};

class NeuralSnake
{
public:
	NeuralSnake(Grid* pGrid, NeuralNetwork* pNeuralNetwork, bool bWrapAround, float fMutationRate, float fTimestep);
	~NeuralSnake();

	/*	@brief 
		@param The time since the last frame
	*/
	bool Update(float fDeltaTime);

	/*	@brief Draws the snake's nodes to screen each frame
		@param Renderer2D to be drawn to
	*/
	void Draw(aie::Renderer2D* pRenderer);

	/*	@brief Gets the nodes that the snake is inhabiting
		@return The nodes that the snake is inhabiting
	*/
	std::vector<Node*> GetSnakeNodes() { return m_pSnakeNodes; }

	/*	@brief Gets the size of the snake
		@return The size of the snake
	*/
	int GetSize() { return m_nSize; }
	
	/*	@brief Gets the move count of the snake
		@return The number of times the snake has moved
	*/
	int GetMoves() { return m_nMoveCount; }

	void SeedRandom(unsigned int nSeed) { srand(nSeed); }

	NeuralNetwork* GetNeuralNetwork() { return m_pNeuralNetwork; }
	void SetTimestep(float fTimestep) { m_fTimestep = fTimestep; m_fTimeToNextMove = fTimestep; }

private:
	// stores a reference to the playing grid
	Grid* m_pGrid;
	// holds all the nodes the snake is inhabiting
	std::vector<Node*> m_pSnakeNodes;
	// the next direction to move in
	eNeuralDirection m_nextDirection;
	// the last direction the snake moved
	eNeuralDirection m_lastDirection;
	// the coordinates of the head node of the snake
	glm::vec2 m_v2HeadNode;
	// the timer until the snake moves next
	float m_fTimeToNextMove;
	// whether the snake is increasing in size this frame or not
	bool m_bIncreasingSize = false;
	// current size of the snake
	int m_nSize = 1;
	// whether the snake can wrap around the borders of the screen or not
	bool m_bWrapAround = false;

	// neural net
	NeuralNetwork* m_pNeuralNetwork;

	int m_nMoveCount = 0;

	bool m_bSnakeNodes[GRID_WIDTH * GRID_HEIGHT];

	// time between each snake movement (seconds)
	float m_fTimestep = 0.1f;
	// scale of the white part of the snake's eye
	const float EYE_WHITE_SCALE = 0.1f;
	// sclae of the black part of the snake's eye
	const float EYE_BLACK_SCALE = 0.05f;
};