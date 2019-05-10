#include "Layer.h"
#include "Matrix.h"
Layer::Layer(int nNeuronCount, int nLastLayerNeuronCount)
{
	m_nNeuronCount = nNeuronCount;
	m_pWeights = new Matrix(nNeuronCount, nLastLayerNeuronCount);
	m_pBiases = new Matrix(nNeuronCount, 1);
}

Layer::Layer(Layer& layer)
{
	m_nNeuronCount = layer.m_nNeuronCount;
	m_pWeights = new Matrix(*layer.m_pWeights);
	m_pBiases = new Matrix(*layer.m_pBiases);
}

Layer::~Layer()
{
	delete m_pWeights;
	m_pWeights = nullptr;
	delete m_pBiases;
	m_pBiases = nullptr;
}