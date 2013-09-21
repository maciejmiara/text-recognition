#include "Network.h"

#include <qglobal.h>
#include <QTime>
#include <math.h>

Network::Network()
{
	neuronsInLayer[0] = NEURONS_INPUT;
	neuronsInLayer[1] = NEURONS_HIDDEN;
	neuronsInLayer[2] = NEURONS_OUTPUT;

	expectedOutput = new double*[NEURONS_OUTPUT];
	for (int i = 0; i < NEURONS_OUTPUT; i++)
		expectedOutput[i] = new double[NEURONS_OUTPUT];

	for (int i = 0; i < NEURONS_OUTPUT; i++)
		for (int j = 0; j < NEURONS_OUTPUT; j++)
			expectedOutput[i][j] = (i == j) ? (double)1.0 : (double)0.0;

	weights = new double**[LAYERS_NUM-1];

	for (int i = 1; i < LAYERS_NUM; i++)
		weights[i-1] = new double*[neuronsInLayer[i]];

	for (int i = 1; i < LAYERS_NUM; i++)
		for (int j = 0; j < neuronsInLayer[i]; j++)
			weights[i-1][j] = new double[neuronsInLayer[i-1]];

	previousWeights = new double**[LAYERS_NUM-1];

	for (int i = 1; i < LAYERS_NUM; i++)
		previousWeights[i-1] = new double*[neuronsInLayer[i]];

	for (int i = 1; i < LAYERS_NUM; i++)
		for (int j = 0; j < neuronsInLayer[i]; j++)
			previousWeights[i-1][j] = new double[neuronsInLayer[i-1]];

	tempWeights = new double**[LAYERS_NUM-1];

	for (int i = 1; i < LAYERS_NUM; i++)
		tempWeights[i-1] = new double*[neuronsInLayer[i]];

	for (int i = 1; i < LAYERS_NUM; i++)
		for (int j = 0; j < neuronsInLayer[i]; j++)
			tempWeights[i-1][j] = new double[neuronsInLayer[i-1]];

	input = new double*[LAYERS_NUM-1];
	for (int i = 0; i < LAYERS_NUM-1; i++)
		input[i] = new double[neuronsInLayer[i]];

	output = new double*[LAYERS_NUM];
	for (int i = 0; i < LAYERS_NUM; i++)
		output[i] = new double[neuronsInLayer[i]];

	outputError = new double[NEURONS_OUTPUT];

	errors = new double*[LAYERS_NUM-1];
	for (int i = 0; i < LAYERS_NUM-1; i++)
		errors[i] = new double[neuronsInLayer[i+1]];
}

Network::~Network()
{
	for (int i = 0; i < NEURONS_OUTPUT; i++)
		delete [] expectedOutput[i];
	delete [] expectedOutput;

	for (int i = 1; i < LAYERS_NUM; i++)
		for (int j = 0; j < neuronsInLayer[i]; j++)
			delete [] weights[i-1][j];
	for (int i = 1; i < LAYERS_NUM; i++)
		delete [] weights[i-1];
	delete [] weights;

	for (int i = 1; i < LAYERS_NUM; i++)
		for (int j = 0; j < neuronsInLayer[i]; j++)
			delete [] previousWeights[i-1][j];
	for (int i = 1; i < LAYERS_NUM; i++)
		delete [] previousWeights[i-1];
	delete [] previousWeights;

	for (int i = 1; i < LAYERS_NUM; i++)
		for (int j = 0; j < neuronsInLayer[i]; j++)
			delete [] tempWeights[i-1][j];
	for (int i = 1; i < LAYERS_NUM; i++)
		delete [] tempWeights[i-1];
	delete [] tempWeights;

	for (int i = 0; i < LAYERS_NUM-1; i++)
		delete [] input[i];
	delete [] input;

	for (int i = 0; i < LAYERS_NUM; i++)
		delete [] output[i];
	delete [] output;

	delete [] outputError;

	for (int i = 0; i < LAYERS_NUM-1; i++)
		delete [] errors[i];
	delete [] errors;
}

double Network::vectorLength(int amount, double* vector)
{
	double sum = 0.0;

	for (int i = 0; i < amount; i++)
	{
		sum += vector[i] * vector[i];
	}

	return sqrt(sum);
}

double Network::normalizeVector(int amount, double* vector)
{
	double length = vectorLength(amount, vector);

	if (length*length < 1.e-30) {
		length = sqrt(1.e-30);
	}

	return 1.0 / length;
}

void Network::normalizeWeights(int amount, double* weights)
{
	double length = normalizeVector(amount, weights);

	for (int i = 0; i < amount; i++)
		weights[i] *= length;
}

void Network::clearWeights()
{
	for (int i = 0; i < LAYERS_NUM-1; i++)
		for (int j = 0; j < neuronsInLayer[i+1]; j++)
			for (int k = 0; k < neuronsInLayer[i]; k++)
			{
				weights[i][j][k] = 0.0;
				previousWeights[i][j][k] = 0.0;
			}
}

void Network::generateWeights()
{
	QTime time = QTime::currentTime();
	qsrand((uint)time.msec());

	for (int i = 0; i < LAYERS_NUM-1; i++)
		for (int j = 0; j < neuronsInLayer[i+1]; i++)
		{
			for (int k = 0; k < neuronsInLayer[i]; k++)
				weights[i][j][k] = qrand() + qrand() - qrand() - qrand();
			normalizeWeights(neuronsInLayer[i], weights[i][j]);
		}
}

void Network::setInputData(double* data, int expectedResult)
{
	currentExpectedResult = expectedResult;

	for (int i = 0; i < NEURONS_INPUT; i++)
		output[0][i] = data[i];
}

void Network::learnPattern()
{
	for (int i = 1; i < LAYERS_NUM; i++)
		for (int j = 0; j < neuronsInLayer[i]; j++)
		{
			input[i-1][j] = 0.0;
			for (int k = 0; k < neuronsInLayer[i-1]; k++)
				input[i-1][j] += output[i-1][k] * weights[i-1][j][k];
			output[i][j] = 1.0 / (1.0 + exp(BETA * (-input[i-1][j])));
		}
}

void Network::calculateOutputError()
{
	for (int i = 0; i < NEURONS_OUTPUT; i++)
		outputError[i] = expectedOutput[currentExpectedResult][i] - output[LAYERS_NUM-1][i];
}

void Network::backPropagateErrors()
{
	for (int i = 0; i < neuronsInLayer[2]; i++)
		errors[1][i] = (expectedOutput[currentExpectedResult][i] - output[2][i]) * output[2][i] * (1.0 - output[2][i]);

	for (int i = 0; i < neuronsInLayer[1]; i++)
	{
		errors[0][i] = 0.0;
		for (int k = 0; k < neuronsInLayer[2]; k++)
			errors[0][i] += output[1][i] * (1.0 - output[1][i]) * errors[1][k] * weights[1][k][i];
	}
}

void Network::adaptWeights()
{
	for (int i = 0; i < LAYERS_NUM-1; i++)
		for (int j = 0; j < neuronsInLayer[i+1]; j++)
			for (int k = 0; k < neuronsInLayer[i]; k++)
			{
				tempWeights[i][j][k] = weights[i][j][k];
				weights[i][j][k] += ETA * errors[i][j] * output[i][k] + ALFA * (weights[i][j][k] - previousWeights[i][j][k]);
				previousWeights[i][j][k] = tempWeights[i][j][k];
			}
}

void Network::learn(int amount, TrainingSet* set, int* expectedResults)
{
	clearWeights();
	generateWeights();
	
	double iterationError = 0.0;
	double epochError = 0.0;

	// 400 epok, po 200 prezentacji ka¿dej próbki w danej epoce
	for (int i = 0; i < MAX_LEARNING_EPOCHS; i++)
	{
		for (int j = 0; j < amount; j++)
			for (int k = 0; k < 200; k++)
			{
				setInputData(set->getSet(j), expectedResults[j]);
				learnPattern();
				calculateOutputError();
				backPropagateErrors();
				adaptWeights();
				for (int j = 0; j < neuronsInLayer[2]; j++)
					iterationError += (expectedOutput[currentExpectedResult][j] - output[LAYERS_NUM - 1][j]) * (expectedOutput[currentExpectedResult][j] - output[LAYERS_NUM - 1][j]);
			}
		epochError = sqrt(iterationError/(double)(amount * neuronsInLayer[LAYERS_NUM - 1]));
		iterationError = 0.0;

		if (epochError < QUIT_ERROR)
			break;
	}
}

int Network::recognize(double* pattern)
{
	setInputData(pattern, 0);
	learnPattern();

	double bestOutput = 0.0;
	int bestChoice = 0;

	for (int i = 0; i < NEURONS_OUTPUT; i++)
		if (output[2][i] > bestOutput)
		{
			bestOutput = output[2][i];
			bestChoice = i;
		}

	return bestChoice;
}

double*** Network::getWeights()
{
	return weights;
}

int* Network::getNeuronsInLayer()
{
	return neuronsInLayer;
}