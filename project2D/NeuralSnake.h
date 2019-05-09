#pragma once
#include <vector>
#include "Renderer2D.h"
#include <glm/glm.hpp>
class Grid;
class Pickup;
class Application2D;
class NeuralNetwork;
struct Node;

// the directions in which the snake can move in
enum class eDirection
{
	Up = 0,
	Right,
	Down,
	Left
};

class NeuralSnake
{
public:
	NeuralSnake(Grid* pGrid, bool bWrapAround);
	~NeuralSnake();

	/*	@brief Gets directional input each frame and moves the snake each time step,
		 increases size of snake when colliding with pickup
		@param The time since the last frame
		@param Reference to the pickup
	*/
	void Update(float fDeltaTime, Pickup* pPickup);

	/*	@brief Draws the snake's nodes to screen each frame
		@param Renderer2D to be drawn to
	*/
	void Draw(aie::Renderer2D* pRenderer);

	/*	@brief Gets the nodes that the snake is inhabiting
		@return The nodes that the snake is inhabiting
	*/
	std::vector<Node*> GetSnakeNodes() { return m_pSnakeNodes; }

private:
	// stores a reference to the playing grid
	Grid* m_pGrid;
	// holds all the nodes the snake is inhabiting
	std::vector<Node*> m_pSnakeNodes;
	// the next direction to move in
	eDirection m_nextDirection;
	// the last direction the snake moved
	eDirection m_lastDirection;
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

	// time between each snake movement (seconds)
	const float TIME_BETWEEN_MOVEMENTS = 0.4f;
	// scale of the white part of the snake's eye
	const float EYE_WHITE_SCALE = 0.1f;
	// sclae of the black part of the snake's eye
	const float EYE_BLACK_SCALE = 0.05f;
};