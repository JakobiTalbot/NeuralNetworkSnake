#pragma once
#include <vector>
class Matrix;
class Layer
{
public:
	Layer(int nNeuronCount, int nLastLayerNeuronCount = 0);
	~Layer();

	Matrix* GetWeightMatrix() { return m_pWeights; }
	Matrix* GetBiasMatrix() { return m_pBiases; }
	int GetNeuronCount() { return m_nNeuronCount; }

private:
	int m_nNeuronCount;
	Matrix* m_pWeights;
	Matrix* m_pBiases;
};