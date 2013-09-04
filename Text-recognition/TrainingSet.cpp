#include <qglobal.h>
#include "globals.h"
#include "TrainingSet.h"

void TrainingSet::insertData(int set, int index, double value)
{
	data[set][index] = value;
}

void TrainingSet::insertSet(int set, double* setData)
{
	memcpy(data[set], setData, NEURONS_NUM * sizeof(double));
}

double TrainingSet::getData(int set, int index)
{
	return data[set][index];
}

double* TrainingSet::getSet(int set)
{
	return data[set];
}