#include <qimage.h>
#include <qrgb.h>
#include "LetterCropper.h"

QImage LetterCropper::crop(QImage& image)
{
	int topMargin = 0, bottomMargin = 0, leftMargin = 0, rightMargin = 0;
	bool marginFound = false;

	for (int i = 0; i < image.height(); i++)
	{
		for (int j = 0; j < image.width(); j++) {
			int test = qGray(image.pixel(j, i));
			if (qGray(image.pixel(j, i)) < 100) {
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
			if (qGray(image.pixel(j, i)) < 100) {
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
			if (qGray(image.pixel(i, j)) < 100) {
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
			if (qGray(image.pixel(i, j)) < 100) {
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