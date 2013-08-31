#include <qimage.h>
#include <qrgb.h>
#include "LetterAnalyzer.h"
#include "globals.h"

QImage LetterAnalyzer::crop(QImage& image)
{
	int topMargin = 0, bottomMargin = 0, leftMargin = 0, rightMargin = 0;
	bool marginFound = false;

	for (int i = 0; i < image.height(); i++)
	{
		for (int j = 0; j < image.width(); j++) {
			int test = qGray(image.pixel(j, i));
			if (qGray(image.pixel(j, i)) < MAXIMUM_LETTER_GRAYNESS) {
				marginFound = true;
				topMargin = i;
				break;
			}
		}
		if (marginFound == true) {
			marginFound = false;
			break;
		}
	}

	for (int i = image.height()-1; i >= 0; i--)
	{
		for (int j = 0; j < image.width(); j++)
			if (qGray(image.pixel(j, i)) < MAXIMUM_LETTER_GRAYNESS) {
				marginFound = true;
				bottomMargin = i;
				break;
			}
		if (marginFound == true) {
			marginFound = false;
			break;
		}
	}

	for (int i = 0; i < image.width(); i++)
	{
		for (int j = 0; j < image.height(); j++)
			if (qGray(image.pixel(i, j)) < MAXIMUM_LETTER_GRAYNESS) {
				marginFound = true;
				leftMargin = i;
				break;
			}
		if (marginFound == true) {
			marginFound = false;
			break;
		}
	}

	for (int i = image.width()-1; i >= 0; i--)
	{
		for (int j = 0; j < image.height(); j++)
			if (qGray(image.pixel(i, j)) < MAXIMUM_LETTER_GRAYNESS) {
				marginFound = true;
				rightMargin = i;
				break;
			}
		if (marginFound == true) {
			marginFound = false;
			break;
		}
	}

	return image.copy(leftMargin, topMargin, rightMargin - leftMargin + 1, bottomMargin - topMargin + 1); 
}

float* LetterAnalyzer::parse(QImage& image)
{
	float* dataVector = new float[NEURONS_NUM];

	if (image.height() < NEURONS_VERTICAL || image.width() < NEURONS_HORIZONTAL)
	{
		image = image.scaled(NEURONS_HORIZONTAL, NEURONS_VERTICAL, Qt::KeepAspectRatio);
	}

	float verticalPartSize = image.height() / (float)NEURONS_VERTICAL;
	float horizontalPartSize = image.width() / (float)NEURONS_HORIZONTAL;
	float tester;
	for (int i = 0; i < NEURONS_VERTICAL; i++)
		for (int j = 0; j < NEURONS_HORIZONTAL; j++)
		{
			dataVector[i*NEURONS_HORIZONTAL+j] = (LetterAnalyzer::isPieceofLetter(image, j*horizontalPartSize, i*verticalPartSize, horizontalPartSize, verticalPartSize)) ? MAX_INPUT : MIN_INPUT;
			tester = dataVector[i*NEURONS_HORIZONTAL+j];
			int tester1 = j*horizontalPartSize;
			int tester2 = i*verticalPartSize;
		}	

	int test = 0;
	return dataVector;
}

bool LetterAnalyzer::isPieceofLetter(QImage& image, int x, int y, int width, int height)
{
	for (int i = y; i < y + height; i++)
		for (int j = x; j < x + width; j++)
			if (qGray(image.pixel(j, i)) < MAXIMUM_LETTER_GRAYNESS)
				return true;
	return false;
}