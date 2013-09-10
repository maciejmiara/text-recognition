#pragma once

#include <qimage.h>

class LetterAnalyzer
{
	public:
		static QImage crop(QImage& image);
		static double* parse(QImage& image);
		static bool isPieceofLetter(QImage& image, int x, int y, int width, int height);
};