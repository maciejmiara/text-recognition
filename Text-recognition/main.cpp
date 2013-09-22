#include "app.h"
#include "LetterAnalyzer.h"
#include "Network.h"
#include "Contour.h"
#include "TrainingSet.h"
#include <QtWidgets/QApplication>
#include <qimage.h>
#include <qimagereader.h>
#include <qimagewriter.h>
#include <qfile.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	App w;
	w.init();
	w.show();
	return a.exec();
}
