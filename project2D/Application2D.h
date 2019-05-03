#pragma once

#include "Application.h"
#include "Renderer2D.h"

class Grid;
class Snake;
class Pickup;
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
	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	Grid*				m_pGrid;
	Snake*				m_pSnake;
	Pickup*				m_pPickup;

	static Application2D* m_pInstance;
};