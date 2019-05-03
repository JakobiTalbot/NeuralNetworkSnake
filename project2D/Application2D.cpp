#include "Application2D.h"
#include "Grid.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Snake.h"
#include "Pickup.h"
Application2D* Application2D::m_pInstance = nullptr;
Application2D::Application2D()
{
	m_2dRenderer = nullptr;
	m_font = nullptr;
	m_pGrid = nullptr;
	m_pSnake = nullptr;
	m_pPickup = nullptr;
}

Application2D::~Application2D()
{

}

bool Application2D::startup()
{
	m_pInstance = this;
	// initialise grid
	m_pGrid = new Grid();
	m_pGrid->Init(getWindowWidth(), getWindowHeight());
	// create snake
	m_pSnake = new Snake(m_pGrid);
	// create renderer
	m_2dRenderer = new aie::Renderer2D();
	// initialise pickup
	m_pPickup = new Pickup(m_pGrid);
	m_pPickup->SpawnPickup(m_pSnake);
	// load font
	m_font = new aie::Font("./font/consolas.ttf", 32);
	return true;
}

void Application2D::shutdown()
{	
	// deallocate pointers
	delete m_2dRenderer;
	delete m_font;
	delete m_pGrid;
	delete m_pSnake;
	delete m_pPickup;
}

void Application2D::update(float deltaTime)
{
	// get input
	aie::Input* input = aie::Input::getInstance();
	
	// update snake
	m_pSnake->Update(deltaTime, m_pPickup);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application2D::draw()
{

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();
	
	// draw objects
	m_pGrid->Draw(m_2dRenderer);
	m_pSnake->Draw(m_2dRenderer);
	m_pPickup->Draw(m_2dRenderer);

	m_2dRenderer->setRenderColour(1, 1, 1);
	// output some text, uses the last used colour
	//char fps[32];
	//sprintf_s(fps, 32, "FPS: %i", getFPS());
	//m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	//m_2dRenderer->drawText(m_font, "Press ESC to quit!", 0, 720 - 64);

	// done drawing sprites
	m_2dRenderer->end();
}