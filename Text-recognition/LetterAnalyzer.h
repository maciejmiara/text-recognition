#include <qimage.h>

class LetterAnalyzer
{
	public:
		static QImage crop(QImage& image);
		static float* parse(QImage& image);
		static bool isPieceofLetter(QImage& image, int x, int y, int width, int height);
};