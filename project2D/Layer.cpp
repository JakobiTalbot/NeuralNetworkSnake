#include "Layer.h"
#include "Matrix.h"
Layer::Layer(int nNeuronCount, int nLastLayerNeuronCount = NULL)
{
	if (nLastLayerNeuronCount != NULL)
	{
		m_pWeights = new Matrix(nNeuronCount, nLastLayerNeuronCount);
		m_pBiases = new Matrix(nNeuronCount, 1);
	}
}

Layer::~Layer()
{
	if (!m_pWeights)
		return;

	delete m_pWeights;
	delete m_pBiases;
}