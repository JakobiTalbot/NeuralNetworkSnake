#include "NeuralNetwork.h"
#include "Layer.h"
#include "Neuron.h"
#include "Matrix.h"
NeuralNetwork::NeuralNetwork(int nInputLayerNeuronCount, int nHiddenLayerCount, int nHiddenLayerNeuronCount, int nOutputLayerNeuronCount, float fLearningRate = 0.1f)
{
	// create input layer
	m_pLayers.push_back(new Layer(nInputLayerNeuronCount));

	// create hidden layers
	m_pLayers.push_back(new Layer(nHiddenLayerNeuronCount, nInputLayerNeuronCount));
	for (int i = 1; i < nHiddenLayerCount; ++i)
		m_pLayers.push_back(new Layer(nHiddenLayerNeuronCount, nHiddenLayerNeuronCount));

	// create output layer
	m_pLayers.push_back(new Layer(nOutputLayerNeuronCount, nHiddenLayerNeuronCount));

	// randomly set weight and bias matrices for all layers except input layer
	for (int i = 1; i < m_pLayers.size; ++i)
	{		
		m_pLayers[i]->GetWeightMatrix()->randomize();
		m_pLayers[i]->GetBiasMatrix()->randomize();
	}
	m_fLearningRate = fLearningRate;
}

NeuralNetwork::~NeuralNetwork()
{
	for (auto& layer : m_pLayers)
		delete layer;
	m_pLayers.clear();
}

void NeuralNetwork::Init()
{

}

float NeuralNetwork::Sigmoid(float x)
{
	float ex = expf(x);
	return ex / (ex + 1);
}