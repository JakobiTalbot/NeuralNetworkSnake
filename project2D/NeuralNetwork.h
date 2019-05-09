#pragma once
#include <vector>
class Layer;
class NeuralNetwork
{
public:
	NeuralNetwork(int nInputLayerNeuronCount, int nHiddenLayerCount, int nHiddenLayerNeuronCount, int nOutputLayerNeuronCount, float fLearningRate = 0.1f);
	~NeuralNetwork();

	void Init();

	float Sigmoid(float x);

	float GetLearningRate() { return m_fLearningRate; }
	void SetLearningRate(float fLearningRate) { m_fLearningRate = fLearningRate; }

private:
	std::vector<Layer*> m_pLayers;
	float m_fLearningRate;
};