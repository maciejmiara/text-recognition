#pragma once

#include "globals.h"

class TrainingSet
{
	private:
		double data[NEURONS_OUTPUT][NEURONS_INPUT];

	public:
		void insertData(int set, int index, double value);
		double getData(int set, int index);
}