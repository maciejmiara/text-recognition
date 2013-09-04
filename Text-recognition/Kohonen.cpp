#include "Kohonen.h"
#include <qglobal.h>
#include <QTime>

void Kohonen::resetWeights()
{
	for (int i = 0; i < NEURONS_OUTPUT; i++)
		for (int j = 0; j < NEURONS_INPUT; j++)
			outWeights[i][j] = 0.0;
}

void Kohonen::generateWeights()
{
	QTime time = QTime::currentTime();
	qsrand((uint)time.msec());

	for (int i = 0; i < NEURONS_OUTPUT; i++)
		for (int j = 0; j < NEURONS_INPUT; j++)
			outWeights[i][j] = qrand() + qrand() - qrand() - qrand();
}

double Kohonen::vectorLength(int amount, double* vector)
{
	double sum = 0.0;

	for (int i = 0; i < amount; i++)
	{
		sum += vector[i] * vector[i];
	}

	return sqrt(sum);
}

double Kohonen::normalizeVector(int amount, double* vector)
{
	double length = vectorLength(amount, vector);

	if (length*length < 1.e-30) {
		length = sqrt(1.e-30);
	}

	return 1.0 / length;
}

double Kohonen::dotProduct(int amount, double* vector1, double* vector2)
{
	double sum = 0.0;

	for (int i = 0; i < amount; i++)
		sum += vector1[i] * vector2[i];

	return sum;
}

void Kohonen::normalizeWeights(int amount, double* weights)
{
	double length = normalizeVector(amount, weights);

	for (int i = 0; i < amount; i++)
		weights[i] *= length;
}

void Kohonen::init()
{
	error = 1.0;
	resetWeights();
	generateWeights();

	for (int i = 0; i < NEURONS_OUTPUT; i++)
		normalizeWeights(NEURONS_INPUT, outWeights[i]);
}

double Kohonen::calculateError(TrainingSet* set, int* winners, double** corrections)
{
	for (int i = 0; i < NEURONS_OUTPUT; i++)
		for (int j = 0; j < NEURONS_INPUT; j++)
			corrections[i][j] = 0.0;

	double biggestError = 0.0;
	double errorVector[NEURONS_INPUT];
	double errorVectorLength = 0.0;
	double difference = 0.0;
	double normalizationFactor = 0.0;
	int winner = 0;
	for (int i = 0; i < NEURONS_OUTPUT; i++)
		winners[i] = 0;

	for (int i = 0; i < NEURONS_OUTPUT; i++)
	{
		winner = pickWinner(set, i, normalizationFactor); 
		++winners[winner];
		errorVectorLength = 0.0;

		for (int j = 0; j < NEURONS_INPUT; j++)
		{
			difference = set->getData(i, j) * normalizationFactor - outWeights[winner][j];
			corrections[winner][j] += difference;
			errorVector[j] = difference;
		}

		errorVectorLength = vectorLength(NEURONS_INPUT, errorVector);

		if (errorVectorLength*errorVectorLength > biggestError)
			biggestError = errorVectorLength;
	}

	return biggestError;
}

int Kohonen::pickWinner(TrainingSet* sets, int set, double& normalizationFactor)
{
	normalizationFactor = normalizeVector(NEURONS_INPUT, sets->getSet(set));

	double best = 0.0;
	int winner = 0;
	for (int i = 0; i < NEURONS_OUTPUT; i++)
	{
		output[i] = dotProduct(NEURONS_INPUT, sets->getSet(set), outWeights[i]) * normalizationFactor;

		output[i] = 0.5 * (output[i] + 1.0) ;

		if (output[i] > best)
		{
			best = output[i];
			winner = i;
		}

		// account for rounding
      if ( output[i] > 1.0 )
         output[i] = 1.0;
      if ( output[i] < 0.0 )
         output[i] = 0.0;
	}

	return winner;
}

void Kohonen::pickUniqueWinner(TrainingSet* set, int* winners)
{
	double distance = 1.e20;
	double normalizationFactor = 0.0;
	int winner = 0;
	int bestMatch = 0;

	for (int i = 0; i < NEURONS_OUTPUT; i++)
	{
		winner = pickWinner(set, i, normalizationFactor);
		if (output[winner] < distance)
		{
			distance = output[winner];
			bestMatch = i;
		}
	}

	winner = pickWinner(set, bestMatch, normalizationFactor);
	double* bestMatchSet = set->getSet(bestMatch);

	distance = -1.e20;

	for (int i = 0; i < NEURONS_OUTPUT; i++)
	{
		if (winners[i] != 0)
			continue;
		if (output[i] > distance)
		{
			distance = output[i];
			bestMatch = i;
		}
		
	}

	memcpy(outWeights[bestMatch], bestMatchSet, NEURONS_INPUT * sizeof(double));
	normalizeWeights(NEURONS_INPUT, outWeights[bestMatch]);
}

double Kohonen::adjustWeights(double rate, int* winners, double** corrections)
{
	double biggestCorrection = 0.0;
	double correction = 0.0;
	double factor = 0.0;
	double length = 0.0;

	for (int i = 0; i < NEURONS_OUTPUT; i++)
	{
		if (winners[i] == 0)
			continue;
		factor = 1.0 / (double) winners[i] * rate;

		for (int j = 0; j < NEURONS_INPUT; j++)
		{
			correction = factor * corrections[i][j];
			outWeights[i][j] += correction;
			length += correction * correction;
		}

		if (length > biggestCorrection)
			biggestCorrection = length;
	}

	return sqrt(biggestCorrection) / rate;
}

bool Kohonen::learn(TrainingSet* set)
{
	int winners[NEURONS_OUTPUT];
	int winnersAmount = 0;
	int winnersNum = 0;
	int retries = 0;
	double rate = LEARN_RATE;
	double** corrections;

	corrections = new double*[NEURONS_OUTPUT];

	for (int i = 0; i < NEURONS_OUTPUT; i++) {
		corrections[i] = new double[NEURONS_INPUT];
		winners[i] = 0;
	}

	error = 1.0;
	for (int i = 0; i < NEURONS_OUTPUT; i++)
		if (vectorLength(NEURONS_INPUT, set->getSet(i)) < 1.e-15)
			return false;

	init();
	double bestError = 1.e30;

	for (int i = 0; ; i++)
	{
		error = calculateError(set, winners, corrections);
		if (error < bestError)
		{
			bestError = error;
			tempError = error;
			memcpy(tempOutWeights, outWeights, NEURONS_OUTPUT * NEURONS_INPUT * sizeof(double));
		}

		winnersAmount = 0;
		for (int j = 0; j < NEURONS_OUTPUT; j++)
			if (winners[j] != 0)
				winnersAmount++;

		if (error < QUIT_ERROR)
			break;

		if (winnersAmount < NEURONS_OUTPUT)
		{
			pickUniqueWinner(set, winners);
			continue;
		}

		double bestCorrection = adjustWeights(rate, winners, corrections);

		if (bestCorrection < 1.e-5)
		{
			if (++retries > MAX_LEARNING_ITERATIONS)
				break;

			init();
			rate = LEARN_RATE;
			continue;
		}

		if (rate > 0.01)
			rate *= REDUCTION;
	}

	error = tempError;
	memcpy(outWeights, tempOutWeights, NEURONS_OUTPUT * NEURONS_INPUT * sizeof(double));

	for (int i = 0; i < NEURONS_OUTPUT; i++)
		normalizeWeights(NEURONS_INPUT, outWeights[i]);

	createMap(set);

	for (int i = 0; i < NEURONS_OUTPUT; i++)
		delete [] corrections[i];
	delete [] corrections;

	return true;
}

void Kohonen::createMap(TrainingSet* set)
{
	int winner = 0;
	double normalizationFactor;

	for (int i = 0; i < NEURONS_OUTPUT; i++)
	{
		winner = pickWinner(set, i, normalizationFactor);
		neuronsMap[winner] = i;
	}
}

int* Kohonen::getMap()
{
	return neuronsMap;
}