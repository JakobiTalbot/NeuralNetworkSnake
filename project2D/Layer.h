#pragma once
#include <vector>
class Matrix;
class Neuron;
class Layer
{
public:
	Layer(int nNeuronCount, int nLastLayerNeuronCount = NULL);
	~Layer();

	Matrix* GetWeightMatrix() { return m_pWeights; }
	Matrix* GetBiasMatrix() { return m_pBiases; }

private:
	std::vector<Neuron*> m_pNeurons;
	Matrix* m_pWeights;
	Matrix* m_pBiases;
};