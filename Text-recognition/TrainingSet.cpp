#include "TrainingSet.h"

void TrainingSet::insertData(int set, int index, double value)
{
	data[set][index] = value;
}

double TrainingSet::getData(int set, int index)
{
	return data[set][index];
}

double* TrainingSet::getSet(int set)
{
	return data[set];
}