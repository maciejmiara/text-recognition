#pragma once

#include "TrainingSet.h"

class Kohonen
{
	protected:
		double output[NEURONS_OUTPUT];
		double outWeights[NEURONS_OUTPUT][NEURONS_INPUT];
		double tempOutWeights[NEURONS_OUTPUT][NEURONS_INPUT];
		double error;
		double tempError;
		int neuronsMap[NEURONS_OUTPUT];

	public:
		void resetWeights();
		void generateWeights();
		double vectorLength(int amount, double* vector);
		double normalizeVector(int amount, double* vector);
		double dotProduct(int amount, double* vector1, double* vector2);
		void normalizeWeights(int amount, double* weights);
		void init();
		double calculateError(TrainingSet* set, int* winners, double** corrections);
		int pickWinner(TrainingSet* sets, int set, double& normalizationFactor);
		void pickUniqueWinner(TrainingSet* set, int* winners);
		double adjustWeights(int* winners, double** corrections);
		bool learn(TrainingSet* set);
		void createMap(TrainingSet* set);
};