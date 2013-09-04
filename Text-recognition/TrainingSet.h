#pragma once

#include "globals.h"

class TrainingSet
{
	private:
		double data[INPUT_DATA][NEURONS_INPUT];

	public:
		void insertData(int set, int index, double value);
		void insertSet(int set, double* data);
		double getData(int set, int index);
		double* getSet(int set);
};