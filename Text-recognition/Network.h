#pragma once

#include "globals.h"
#include "TrainingSet.h"

class Network
{
	private:
		// wagi neuronów
		double*** weights;
		// wagi neuronów z poprzedniej iteracji
		double*** previousWeights;
		// tymczasowe wagi neuronów
		double*** tempWeights;
		// wejœcia neuronów
		double** input;
		// wyjœcia neuronów
		double** output;
		// wyniki oczekiwane przez nauczyciela
		double** expectedOutput;
		// b³êdy poszczególnych neuronów
		double** errors;
		// b³¹d warstwy wyjœciowej
		double* outputError;
		// oczekiwany wynik dla podanej próbki
		int currentExpectedResult;

		// informacja o iloœci neuronów w ka¿dej warstwie
		int neuronsInLayer[LAYERS_NUM];

	public:
		Network();
		~Network();

		// oblicz d³ugoœæ wektora
		double vectorLength(int amount, double* vector);
		// normalizuj wektor
		double normalizeVector(int amount, double* vector);
		// normalizuj wagi
		void normalizeWeights(int amount, double* weights);
		// zresetuj wagi
		void clearWeights();
		// generuj losowe wagi
		void generateWeights();
		// wprowadŸ wzorzec do sieci
		void setInputData(double* data, int expectedResult);
		// uczenie sieci wzorca
		void learnPattern();
		// oblicz b³¹d wyjœciowy
		void calculateOutputError();
		// oblicz b³¹d dla warstw u¿ywaj¹c metody wstecznej propagacji œredniej
		void backPropagateErrors();
		// dopasuj wago neuronów
		void adaptWeights();
		// przeprowadŸ proces nauki na zestawie wzorców
		void learn(int amount, TrainingSet* set, int* expectedResults);
		// rozpoznaj wzorzec
		int recognize(double* pattern);
		// pobierz wagi
		double*** getWeights();
		// pobierz informacjê o iloœci neuronów w warstwie
		int* getNeuronsInLayer();
};
