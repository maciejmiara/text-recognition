#pragma once

#include "globals.h"
#include "TrainingSet.h"

class Network
{
	private:
		double*** weights;
		double*** previousWeights;
		double*** tempWeights;
		double** input;
		double** output;
		double** expectedOutput;
		double** errors;
		double* outputError;
		int currentExpectedResult;

		int neuronsInLayer[LAYERS_NUM];

	public:
		Network();
		~Network();

		double vectorLength(int amount, double* vector);
		double normalizeVector(int amount, double* vector);
		void normalizeWeights(int amount, double* weights);
		void clearWeights();
		void generateWeights();
		void setInputData(double* data, int expectedResult);
		void learnPattern();
		void calculateOutputError();
		void backPropagateErrors();
		void adaptWeights();
		void learn(int amount, TrainingSet* set);
		int recognize(double* pattern);
};
