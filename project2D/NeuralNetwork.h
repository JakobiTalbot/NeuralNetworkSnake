#pragma once
#include <vector>
class Layer;
float Sigmoid(float x);
float Tanh(float x);
class NeuralNetwork
{
public:
	NeuralNetwork(int nInputLayerNeuronCount, int nHiddenLayerCount, int nHiddenLayerNeuronCount, int nOutputLayerNeuronCount);
	NeuralNetwork(NeuralNetwork& network);
	~NeuralNetwork();

	void GetOutput(const float* pInput, float* pOutput);

	void Mutate(float fMutationRate);

	int RouletteSelect(std::vector<int> fitnesses);
	NeuralNetwork* SelectThreshold(std::vector<int> fitnesses, std::vector<NeuralNetwork*> networks, int nFitnessThreshold);

private:
	std::vector<Layer*> m_pLayers;
	int m_nOutputNeuronCount;
};