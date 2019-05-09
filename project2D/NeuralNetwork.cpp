#include "NeuralNetwork.h"
#include "Layer.h"
#include "Matrix.h"
NeuralNetwork::NeuralNetwork(int nInputLayerNeuronCount, int nHiddenLayerCount, int nHiddenLayerNeuronCount, int nOutputLayerNeuronCount, float fLearningRate)
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
	for (int i = 1; i < m_pLayers.size(); ++i)
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

void NeuralNetwork::Guess(const int* pInput, int* pOutput)
{
	Matrix mLastLayer = Matrix(m_pLayers[0]->GetNeuronCount(), 1);

	// set inputs
	for (int i = 0; i < m_pLayers[0]->GetNeuronCount(); ++i)
		mLastLayer[i][0] = pInput[i];

	for (int i = 1; i < m_pLayers.size() - 1; ++i)
	{
		// apply neurons for this layer by weight
		Matrix mLayer = m_pLayers[i]->GetWeightMatrix()->product(mLastLayer);
		// add bias
		mLayer += *(m_pLayers[i]->GetBiasMatrix());
		// apply sigmoid to values in matrix
		mLayer.map(&Sigmoid);
		mLastLayer = mLayer;
	}

	// set output matrix
	for (int i = 0; i < m_pLayers[m_pLayers.size() - 1]->GetNeuronCount(); ++i)
		pOutput[i] = mLastLayer[i][0];
}

void NeuralNetwork::Propagate(const int* pInputs, const int* pTargets)
{
}

void NeuralNetwork::Mutate(float fMutationRate)
{
	for (int i = 1; i < m_pLayers.size(); ++i)
	{
		m_pLayers[i]->GetBiasMatrix()->mutate(fMutationRate);
		m_pLayers[i]->GetWeightMatrix()->mutate(fMutationRate);
	}
}

float Sigmoid(float x)
{
	float ex = expf(x);
	return ex / (ex + 1);
}