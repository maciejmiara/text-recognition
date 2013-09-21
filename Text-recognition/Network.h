#pragma once

#include "globals.h"
#include "TrainingSet.h"

class Network
{
	private:
		// wagi neuron�w
		double*** weights;
		// wagi neuron�w z poprzedniej iteracji
		double*** previousWeights;
		// tymczasowe wagi neuron�w
		double*** tempWeights;
		// wej�cia neuron�w
		double** input;
		// wyj�cia neuron�w
		double** output;
		// wyniki oczekiwane przez nauczyciela
		double** expectedOutput;
		// b��dy poszczeg�lnych neuron�w
		double** errors;
		// b��d warstwy wyj�ciowej
		double* outputError;
		// oczekiwany wynik dla podanej pr�bki
		int currentExpectedResult;

		// informacja o ilo�ci neuron�w w ka�dej warstwie
		int neuronsInLayer[LAYERS_NUM];

	public:
		Network();
		~Network();

		// oblicz d�ugo�� wektora
		double vectorLength(int amount, double* vector);
		// normalizuj wektor
		double normalizeVector(int amount, double* vector);
		// normalizuj wagi
		void normalizeWeights(int amount, double* weights);
		// zresetuj wagi
		void clearWeights();
		// generuj losowe wagi
		void generateWeights();
		// wprowad� wzorzec do sieci
		void setInputData(double* data, int expectedResult);
		// uczenie sieci wzorca
		void learnPattern();
		// oblicz b��d wyj�ciowy
		void calculateOutputError();
		// oblicz b��d dla warstw u�ywaj�c metody wstecznej propagacji �redniej
		void backPropagateErrors();
		// dopasuj wago neuron�w
		void adaptWeights();
		// przeprowad� proces nauki na zestawie wzorc�w
		void learn(int amount, TrainingSet* set, int* expectedResults);
		// rozpoznaj wzorzec
		int recognize(double* pattern);
		// pobierz wagi
		double*** getWeights();
		// pobierz informacj� o ilo�ci neuron�w w warstwie
		int* getNeuronsInLayer();
};
