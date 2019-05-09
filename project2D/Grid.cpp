#include "Grid.h"
#include "Application2D.h"
#include <iostream>
Grid::Grid()
{
	m_pNodes = nullptr;
}

Grid::~Grid()
{
	// deletes 2d array of nodes
	for (int i = 0; i < GRID_HEIGHT; ++i)
	{
		delete[] m_pNodes[i];
	}
	delete[] m_pNodes;
	m_pNodes = nullptr;
}

void Grid::Init()
{
	// initialise node array
	m_pNodes = new Node*[GRID_WIDTH];
	float x = (float)Application2D::GetInstance()->getWindowWidth() / GRID_WIDTH;
	float y = (float)Application2D::GetInstance()->getWindowHeight() / GRID_HEIGHT;
	// nested for loop to create array of nodes
	for (int i = 0; i < GRID_WIDTH; ++i)
	{
		m_pNodes[i] = new Node[GRID_HEIGHT];
		for (int j = 0; j < GRID_HEIGHT; ++j)
		{
			m_pNodes[i][j].m_v2Position = { i * x + (x / 2.0f), j * y + (y / 2.0f)};
			m_pNodes[i][j].m_v2Extents = { x, y };
		}
	}
}

void Grid::Draw(aie::Renderer2D* pRenderer)
{
	pRenderer->setRenderColour(0x2f2f2fff);
	static float x = (float)Application2D::GetInstance()->getWindowWidth() / GRID_WIDTH;
	static float y = (float)Application2D::GetInstance()->getWindowHeight() / GRID_HEIGHT;
	// draw grid
	for (int i = 0; i < GRID_WIDTH; ++i)
	{
		for (int j = 0; j < GRID_HEIGHT; ++j)
		{
			pRenderer->drawBox(m_pNodes[i][j].m_v2Position.x, m_pNodes[i][j].m_v2Position.y,
				m_pNodes[i][j].m_v2Extents.x, m_pNodes[i][j].m_v2Extents.y, 0.f, 100.f);
		}

		// draw grid lines
		// horizontal lines
		pRenderer->drawLine(0, i * y, (float)Application2D::GetInstance()->getWindowWidth(), i * y);
		// vertical lines
		pRenderer->drawLine(i * x, 0, i * x, (float)Application2D::GetInstance()->getWindowHeight());
	}
}