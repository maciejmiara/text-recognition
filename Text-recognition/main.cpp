#include "app.h"
#include "LetterAnalyzer.h"
#include "Contour.h"
#include "TrainingSet.h"
#include "Kohonen.h"
#include <QtWidgets/QApplication>
#include <qimage.h>
#include <qimagereader.h>
#include <qimagewriter.h>
#include <qfile.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;


void cut_letters(Mat image)
{
	int width = 35;
	for (int i=0;i<26;i++)
	{
		Rect myROI(width*i, 0, width, 40);
		Mat croppedImage;
		Mat(image, myROI).copyTo(croppedImage);
		
		stringstream file;
		
		char ch = (char)(i+65); 
        file<<"letters/"<<ch<<"20.jpg";
        imwrite(file.str(),croppedImage);
	}
	
}
int main(int argc, char *argv[])
{

	QString letters[26] = {"training-sets/A1.jpg", "training-sets/B1.jpg", "training-sets/C1.jpg", "training-sets/D1.jpg",
						   "training-sets/E1.jpg", "training-sets/F1.jpg", "training-sets/G1.jpg", "training-sets/H1.jpg",
						   "training-sets/I1.jpg", "training-sets/J1.jpg", "training-sets/K1.jpg", "training-sets/L1.jpg",
						   "training-sets/M1.jpg", "training-sets/N1.jpg", "training-sets/O1.jpg", "training-sets/P1.jpg",
						   "training-sets/Q1.jpg", "training-sets/R1.jpg", "training-sets/S1.jpg", "training-sets/T1.jpg",
						   "training-sets/U1.jpg", "training-sets/V1.jpg", "training-sets/W1.jpg", "training-sets/X1.jpg",
						   "training-sets/Y1.jpg", "training-sets/Z1.jpg"};

	QString letters2[26] = {"training-sets/A2.jpg", "training-sets/B2.jpg", "training-sets/C2.jpg", "training-sets/D2.jpg",
						   "training-sets/E2.jpg", "training-sets/F2.jpg", "training-sets/G2.jpg", "training-sets/H2.jpg",
						   "training-sets/I2.jpg", "training-sets/J2.jpg", "training-sets/K2.jpg", "training-sets/L2.jpg",
						   "training-sets/M2.jpg", "training-sets/N2.jpg", "training-sets/O2.jpg", "training-sets/P2.jpg",
						   "training-sets/Q2.jpg", "training-sets/R2.jpg", "training-sets/S2.jpg", "training-sets/T2.jpg",
						   "training-sets/U2.jpg", "training-sets/V2.jpg", "training-sets/W2.jpg", "training-sets/X2.jpg",
						   "training-sets/Y2.jpg", "training-sets/Z2.jpg"};

	QString letters3[26] = {"training-sets/A3.jpg", "training-sets/B3.jpg", "training-sets/C3.jpg", "training-sets/D3.jpg",
						   "training-sets/E3.jpg", "training-sets/F3.jpg", "training-sets/G3.jpg", "training-sets/H3.jpg",
						   "training-sets/I3.jpg", "training-sets/J3.jpg", "training-sets/K3.jpg", "training-sets/L3.jpg",
						   "training-sets/M3.jpg", "training-sets/N3.jpg", "training-sets/O3.jpg", "training-sets/P3.jpg",
						   "training-sets/Q3.jpg", "training-sets/R3.jpg", "training-sets/S3.jpg", "training-sets/T3.jpg",
						   "training-sets/U3.jpg", "training-sets/V3.jpg", "training-sets/W3.jpg", "training-sets/X3.jpg",
						   "training-sets/Y3.jpg", "training-sets/Z3.jpg"};

	QString letters4[26] = {"training-sets/A4.jpg", "training-sets/B4.jpg", "training-sets/C4.jpg", "training-sets/D4.jpg",
						   "training-sets/E4.jpg", "training-sets/F4.jpg", "training-sets/G4.jpg", "training-sets/H4.jpg",
						   "training-sets/I4.jpg", "training-sets/J4.jpg", "training-sets/K4.jpg", "training-sets/L4.jpg",
						   "training-sets/M4.jpg", "training-sets/N4.jpg", "training-sets/O4.jpg", "training-sets/P4.jpg",
						   "training-sets/Q4.jpg", "training-sets/R4.jpg", "training-sets/S4.jpg", "training-sets/T4.jpg",
						   "training-sets/U4.jpg", "training-sets/V4.jpg", "training-sets/W4.jpg", "training-sets/X4.jpg",
						   "training-sets/Y4.jpg", "training-sets/Z4.jpg"};

	QImage img = QImage();

	TrainingSet set = TrainingSet();
	TrainingSet set2 = TrainingSet();
	TrainingSet set3 = TrainingSet();
	TrainingSet set4 = TrainingSet();
	
	int j = 0;

	QFile file("input_data.txt");
	file.open(QIODevice::WriteOnly | QIODevice::Text);

	for (int i = 0; i < 26; i++)
	{
		if (img.load(letters[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			//cropped.save(letters[i]);
			double* analyzed = LetterAnalyzer::parse(cropped);
			set.insertSet(i, analyzed);
			file.write(letters[i].toLocal8Bit());
			file.write("\n");
			for (int j = 0; j < NEURONS_VERTICAL; j++)
			{
				for (int k = 0; k < NEURONS_HORIZONTAL; k++)
				{
					if (analyzed[j*NEURONS_HORIZONTAL+k] > 0.4)
						file.write("1");
					else
						file.write("0");
				}
				file.write("\n");
			}
					
			//j++;
		}

		if (img.load(letters2[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			//cropped.save(letters[i]);
			double* analyzed = LetterAnalyzer::parse(cropped);
			set2.insertSet(i, analyzed);
			file.write(letters2[i].toLocal8Bit());
			file.write("\n");
			for (int j = 0; j < NEURONS_VERTICAL; j++)
			{
				for (int k = 0; k < NEURONS_HORIZONTAL; k++)
				{
					if (analyzed[j*NEURONS_HORIZONTAL+k] > 0.4)
						file.write("1");
					else
						file.write("0");
				}
				file.write("\n");
			}
		}

		if (img.load(letters3[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			//cropped.save(letters[i]);
			double* analyzed = LetterAnalyzer::parse(cropped);
			set3.insertSet(i, analyzed);
			file.write(letters3[i].toLocal8Bit());
			file.write("\n");
			for (int j = 0; j < NEURONS_VERTICAL; j++)
			{
				for (int k = 0; k < NEURONS_HORIZONTAL; k++)
				{
					if (analyzed[j*NEURONS_HORIZONTAL+k] > 0.4)
						file.write("1");
					else
						file.write("0");
				}
				file.write("\n");
			}
		}

		if (img.load(letters4[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			//cropped.save(letters[i]);
			double* analyzed = LetterAnalyzer::parse(cropped);
			set4.insertSet(i, analyzed);
			file.write(letters4[i].toLocal8Bit());
			file.write("\n");
			for (int j = 0; j < NEURONS_VERTICAL; j++)
			{
				for (int k = 0; k < NEURONS_HORIZONTAL; k++)
				{
					if (analyzed[j*NEURONS_HORIZONTAL+k] > 0.4)
						file.write("1");
					else
						file.write("0");
				}
				file.write("\n");
			}
		}
	}

	file.close();

/*	TrainingSet trainingSets[4];
	trainingSets[0] = set;
	trainingSets[1] = set2;
	trainingSets[2] = set3;
	trainingSets[3] = set4;*/

	Kohonen network = Kohonen();
/*	network.init();*/
	network.learn(&set);
/*	network.learn(&set2);
	network.learn(&set3);
	network.learn(&set4);*/
	double normalizationFactor = 0.0;
	if (img.load("training-sets/P4.jpg"))
	{
		QImage cropped = LetterAnalyzer::crop(img);
		set.insertSet(0, LetterAnalyzer::parse(cropped));
	}

	int winner = network.pickWinner(&set, 0, normalizationFactor);
	int letter = network.getMap()[winner];

	
	Mat image = imread("img/Bookman_Old_Style.jpg",0);  //dodaæ zabezpieczenia: co jak nie siê plik Ÿle otworzy
	cut_letters(image);
	
//	Contour cont;
//	cont.getContour(image);

	QApplication a(argc, argv);
	App w;
	w.show();
	return a.exec();
}
