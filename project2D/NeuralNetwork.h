#pragma once
#include <vector>
class Layer;
float Sigmoid(float x);
class NeuralNetwork
{
public:
	NeuralNetwork(int nInputLayerNeuronCount, int nHiddenLayerCount, int nHiddenLayerNeuronCount, int nOutputLayerNeuronCount, float fLearningRate = 0.1f);
	NeuralNetwork(NeuralNetwork& network);
	~NeuralNetwork();

	void Guess(const float* pInput, float* pOutput);
	void Propagate(const int* pInputs, const int* pTargets);

	void Mutate(float fMutationRate);

	float GetLearningRate() { return m_fLearningRate; }
	void SetLearningRate(float fLearningRate) { m_fLearningRate = fLearningRate; }

	int RouletteSelect(int fitnesses[], int nFitnessesCount);

private:
	std::vector<Layer*> m_pLayers;
	float m_fLearningRate;
};