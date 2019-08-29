#pragma once

#include "Application.h"
#include "Renderer2D.h"

class Grid;
class Snake;
class Pickup;
class NeuralSnakeEnvironment;
class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	static Application2D* GetInstance() { return m_pInstance; }

protected:
	aie::Renderer2D*			m_2dRenderer;
	Grid*						m_pGrid;
	Snake*						m_pSnake;
	NeuralSnakeEnvironment*		m_pSnakeEnvironment;
	Pickup*						m_pPickup;

	bool						m_bDraw = true;

	static Application2D* m_pInstance;
};