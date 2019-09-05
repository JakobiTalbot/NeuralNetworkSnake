#include "NeuralNetwork.h"
#include "Layer.h"
#include "Matrix.h"
#include <math.h>
#include <algorithm>
NeuralNetwork::NeuralNetwork(int nInputLayerNeuronCount, int nHiddenLayerCount, int nHiddenLayerNeuronCount, int nOutputLayerNeuronCount)
{
	// create input layer
	m_pLayers.push_back(new Layer(nInputLayerNeuronCount, nInputLayerNeuronCount));

	// create hidden layers
	m_pLayers.push_back(new Layer(nHiddenLayerNeuronCount, nInputLayerNeuronCount));
	for (int i = 0; i < nHiddenLayerCount - 1; ++i)
		m_pLayers.push_back(new Layer(nHiddenLayerNeuronCount, nHiddenLayerNeuronCount));

	// create output layer
	m_pLayers.push_back(new Layer(nOutputLayerNeuronCount, nHiddenLayerNeuronCount));

	// randomly set weight and bias matrices for all layers except input layer
	for (int i = 0; i < m_pLayers.size(); ++i)
	{		
		m_pLayers[i]->GetWeightMatrix()->randomize();
		m_pLayers[i]->GetBiasMatrix()->randomize();
	}
	m_nOutputNeuronCount = nOutputLayerNeuronCount;
}

NeuralNetwork::NeuralNetwork(NeuralNetwork& network)
{
	m_nOutputNeuronCount = network.m_nOutputNeuronCount;
	for (auto& layer : network.m_pLayers)
		m_pLayers.push_back(new Layer(*layer));
}

NeuralNetwork::~NeuralNetwork()
{
	int size = m_pLayers.size();
	for (int i = 0; i < size; ++i)
		delete m_pLayers[i];
	m_pLayers.clear();
}

void NeuralNetwork::GetOutput(const float* pInput, float* pOutput)
{
	static const int count = 8;
	Matrix m = Matrix(count, 1);
	// set input layer
	for (int i = 0; i < count; ++i)
		m[i][0] = pInput[i];

	// iterate through layers
	for (int i = 0; i < m_pLayers.size(); ++i)
	{
		m = m_pLayers[i]->GetWeightMatrix()->product(m);
		m = m + *m_pLayers[i]->GetBiasMatrix();
		m.map(Tanh);
	}

	// set outputs
	for (int i = 0; i < m_nOutputNeuronCount; ++i)
		pOutput[i] = m[i][0];
}

void NeuralNetwork::Mutate(float fMutationRate)
{
	for (int i = 0; i < m_pLayers.size(); ++i)
	{
		m_pLayers[i]->GetBiasMatrix()->mutate(fMutationRate);
		m_pLayers[i]->GetWeightMatrix()->mutate(fMutationRate);
	}
}

int NeuralNetwork::RouletteSelect(std::vector<int> fitnesses)
{
	// get total fitness
	int nTotalFitness = 0;
	for (int i = 0; i < fitnesses.size(); ++i)
	{
		nTotalFitness += fitnesses[i];
	}

	// select a number between 0 and total fitness
	int selectedNum = rand() % nTotalFitness;
	int lastMaxNum = 0;
	// find which fitness was selected
	for (int i = 0; i < fitnesses.size(); ++i)
	{
		lastMaxNum += fitnesses[i];
		if (lastMaxNum >= selectedNum)
		{
			return i;
		}
	}
	// return last number
	return fitnesses.size() - 1;
}

NeuralNetwork* NeuralNetwork::SelectThreshold(std::vector<int> fitnesses, std::vector<NeuralNetwork*> networks, int nFitnessThreshold)
{
	std::vector<NeuralNetwork*> goodNetworks;
	// cut out fitnesses under threshold
	for (int i = 0; i < fitnesses.size(); ++i)
	{
		if (fitnesses[i] > nFitnessThreshold)
		{
			goodNetworks.push_back(networks[i]);
		}
	}

	// if none meet threshold
	if (goodNetworks.size() == 0)
		return networks[RouletteSelect(fitnesses)];

	// select network above threshold at random
	int iRand = rand() % goodNetworks.size();
	return goodNetworks[iRand];
}

float Sigmoid(float x)
{
	float ex = expf(-x);
	return 1 / (1 + ex);
}

float Tanh(float x)
{
	return (float)tanh(x);
}