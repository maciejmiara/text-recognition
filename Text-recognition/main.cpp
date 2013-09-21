#include "app.h"
#include "LetterAnalyzer.h"
#include "Network.h"
//#include "Contour.h"
#include "TrainingSet.h"
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
/*	QString letters[26] = {"training-sets/A1.jpg", "training-sets/B1.jpg", "training-sets/C1.jpg", "training-sets/D1.jpg",
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

	QString letters5[26] = {"training-sets/A5.jpg", "training-sets/B5.jpg", "training-sets/C5.jpg", "training-sets/D5.jpg",
						   "training-sets/E5.jpg", "training-sets/F5.jpg", "training-sets/G5.jpg", "training-sets/H5.jpg",
						   "training-sets/I5.jpg", "training-sets/J5.jpg", "training-sets/K5.jpg", "training-sets/L5.jpg",
						   "training-sets/M5.jpg", "training-sets/N5.jpg", "training-sets/O5.jpg", "training-sets/P5.jpg",
						   "training-sets/Q5.jpg", "training-sets/R5.jpg", "training-sets/S5.jpg", "training-sets/T5.jpg",
						   "training-sets/U5.jpg", "training-sets/V5.jpg", "training-sets/W5.jpg", "training-sets/X5.jpg",
						   "training-sets/Y5.jpg", "training-sets/Z5.jpg"};

	QString letters6[26] = {"training-sets/A6.jpg", "training-sets/B6.jpg", "training-sets/C6.jpg", "training-sets/D6.jpg",
						   "training-sets/E6.jpg", "training-sets/F6.jpg", "training-sets/G6.jpg", "training-sets/H6.jpg",
						   "training-sets/I6.jpg", "training-sets/J6.jpg", "training-sets/K6.jpg", "training-sets/L6.jpg",
						   "training-sets/M6.jpg", "training-sets/N6.jpg", "training-sets/O6.jpg", "training-sets/P6.jpg",
						   "training-sets/Q6.jpg", "training-sets/R6.jpg", "training-sets/S6.jpg", "training-sets/T6.jpg",
						   "training-sets/U6.jpg", "training-sets/V6.jpg", "training-sets/W6.jpg", "training-sets/X6.jpg",
						   "training-sets/Y6.jpg", "training-sets/Z6.jpg"};

	QString letters7[26] = {"training-sets/A7.jpg", "training-sets/B7.jpg", "training-sets/C7.jpg", "training-sets/D7.jpg",
						   "training-sets/E7.jpg", "training-sets/F7.jpg", "training-sets/G7.jpg", "training-sets/H7.jpg",
						   "training-sets/I7.jpg", "training-sets/J7.jpg", "training-sets/K7.jpg", "training-sets/L7.jpg",
						   "training-sets/M7.jpg", "training-sets/N7.jpg", "training-sets/O7.jpg", "training-sets/P7.jpg",
						   "training-sets/Q7.jpg", "training-sets/R7.jpg", "training-sets/S7.jpg", "training-sets/T7.jpg",
						   "training-sets/U7.jpg", "training-sets/V7.jpg", "training-sets/W7.jpg", "training-sets/X7.jpg",
						   "training-sets/Y7.jpg", "training-sets/Z7.jpg"};

	QString letters8[26] = {"training-sets/A8.jpg", "training-sets/B8.jpg", "training-sets/C8.jpg", "training-sets/D8.jpg",
						   "training-sets/E8.jpg", "training-sets/F8.jpg", "training-sets/G8.jpg", "training-sets/H8.jpg",
						   "training-sets/I8.jpg", "training-sets/J8.jpg", "training-sets/K8.jpg", "training-sets/L8.jpg",
						   "training-sets/M8.jpg", "training-sets/N8.jpg", "training-sets/O8.jpg", "training-sets/P8.jpg",
						   "training-sets/Q8.jpg", "training-sets/R8.jpg", "training-sets/S8.jpg", "training-sets/T8.jpg",
						   "training-sets/U8.jpg", "training-sets/V8.jpg", "training-sets/W8.jpg", "training-sets/X8.jpg",
						   "training-sets/Y8.jpg", "training-sets/Z8.jpg"};

	QString letters9[26] = {"training-sets/A9.jpg", "training-sets/B9.jpg", "training-sets/C9.jpg", "training-sets/D9.jpg",
						   "training-sets/E9.jpg", "training-sets/F9.jpg", "training-sets/G9.jpg", "training-sets/H9.jpg",
						   "training-sets/I9.jpg", "training-sets/J9.jpg", "training-sets/K9.jpg", "training-sets/L9.jpg",
						   "training-sets/M9.jpg", "training-sets/N9.jpg", "training-sets/O9.jpg", "training-sets/P9.jpg",
						   "training-sets/Q9.jpg", "training-sets/R9.jpg", "training-sets/S9.jpg", "training-sets/T9.jpg",
						   "training-sets/U9.jpg", "training-sets/V9.jpg", "training-sets/W9.jpg", "training-sets/X9.jpg",
						   "training-sets/Y9.jpg", "training-sets/Z9.jpg"};

	QString letters10[26] = {"training-sets/A10.jpg", "training-sets/B10.jpg", "training-sets/C10.jpg", "training-sets/D10.jpg",
						   "training-sets/E10.jpg", "training-sets/F10.jpg", "training-sets/G10.jpg", "training-sets/H10.jpg",
						   "training-sets/I10.jpg", "training-sets/J10.jpg", "training-sets/K10.jpg", "training-sets/L10.jpg",
						   "training-sets/M10.jpg", "training-sets/N10.jpg", "training-sets/O10.jpg", "training-sets/P10.jpg",
						   "training-sets/Q10.jpg", "training-sets/R10.jpg", "training-sets/S10.jpg", "training-sets/T10.jpg",
						   "training-sets/U10.jpg", "training-sets/V10.jpg", "training-sets/W10.jpg", "training-sets/X10.jpg",
						   "training-sets/Y10.jpg", "training-sets/Z10.jpg"};

	QString letters11[26] = {"training-sets/A11.jpg", "training-sets/B11.jpg", "training-sets/C11.jpg", "training-sets/D11.jpg",
						   "training-sets/E11.jpg", "training-sets/F11.jpg", "training-sets/G11.jpg", "training-sets/H11.jpg",
						   "training-sets/I11.jpg", "training-sets/J11.jpg", "training-sets/K11.jpg", "training-sets/L11.jpg",
						   "training-sets/M11.jpg", "training-sets/N11.jpg", "training-sets/O11.jpg", "training-sets/P11.jpg",
						   "training-sets/Q11.jpg", "training-sets/R11.jpg", "training-sets/S11.jpg", "training-sets/T11.jpg",
						   "training-sets/U11.jpg", "training-sets/V11.jpg", "training-sets/W11.jpg", "training-sets/X11.jpg",
						   "training-sets/Y11.jpg", "training-sets/Z11.jpg"};
*/
	//image();
	QImage img = QImage();

/*	TrainingSet set = TrainingSet();
	TrainingSet set2 = TrainingSet();
	TrainingSet set3 = TrainingSet();
	TrainingSet set4 = TrainingSet();
	
	int z = 0;

	QFile file("input_data.txt");
	file.open(QIODevice::WriteOnly | QIODevice::Text);

	for (int i = 0; i < 26; i++)
	{
		if (img.load(letters[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			//cropped.save(letters[i]);
			double* analyzed = LetterAnalyzer::parse(cropped);
			set.insertSet(z, analyzed);
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
			z++;
		}

		if (img.load(letters2[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			//cropped.save(letters[i]);
			double* analyzed = LetterAnalyzer::parse(cropped);
			set.insertSet(z, analyzed);
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
			z++;
		}

		if (img.load(letters3[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			//cropped.save(letters[i]);
			double* analyzed = LetterAnalyzer::parse(cropped);
			set.insertSet(z, analyzed);
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
			z++;
		}

		if (img.load(letters4[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			//cropped.save(letters[i]);
			double* analyzed = LetterAnalyzer::parse(cropped);
			set.insertSet(z, analyzed);
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
			z++;
		}

		if (img.load(letters5[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			//cropped.save(letters[i]);
			double* analyzed = LetterAnalyzer::parse(cropped);
			set.insertSet(z, analyzed);
			file.write(letters5[i].toLocal8Bit());
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
			z++;
		}

		if (img.load(letters6[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			//cropped.save(letters[i]);
			double* analyzed = LetterAnalyzer::parse(cropped);
			set.insertSet(z, analyzed);
			file.write(letters6[i].toLocal8Bit());
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
			z++;
		}

/*		if (img.load(letters7[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			//cropped.save(letters[i]);
			double* analyzed = LetterAnalyzer::parse(cropped);
			set.insertSet(z, analyzed);
			file.write(letters7[i].toLocal8Bit());
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
			z++;
		}

		if (img.load(letters8[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			//cropped.save(letters[i]);
			double* analyzed = LetterAnalyzer::parse(cropped);
			set.insertSet(z, analyzed);
			file.write(letters8[i].toLocal8Bit());
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
			z++;
		}

		if (img.load(letters9[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			//cropped.save(letters[i]);
			double* analyzed = LetterAnalyzer::parse(cropped);
			set.insertSet(z, analyzed);
			file.write(letters9[i].toLocal8Bit());
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
			z++;
		}

		if (img.load(letters10[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			//cropped.save(letters[i]);
			double* analyzed = LetterAnalyzer::parse(cropped);
			set.insertSet(z, analyzed);
			file.write(letters10[i].toLocal8Bit());
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
			z++;
		}
	}

	file.close();

/*	TrainingSet trainingSets[4];
	trainingSets[0] = set;
	trainingSets[1] = set2;
	trainingSets[2] = set3;
	trainingSets[3] = set4;*/

/*	network.init();*/
//	network.learn(&set);
	//network.learn(&set2);
/*	network.learn(&set3);
	network.learn(&set4);*/

//	Network* network = new Network();
//	network->learn(156, &set);
/*	double normalizationFactor = 0.0;
	int testLetters[26];
	for (int i = 0; i < 26; i++)
	{
		if (img.load(letters11[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			set.insertSet(0, LetterAnalyzer::parse(cropped));
			//cropped.save("training-sets/test/Z.jpg");
//			testLetters[i] = network->recognize(set.getSet(0));
		}
	}
	letters[0];

	for (int i = 0; i < 26; i++)
	{
		if (img.load(letters8[i]))
		{
			QImage cropped = LetterAnalyzer::crop(img);
			set.insertSet(0, LetterAnalyzer::parse(cropped));
			//cropped.save("training-sets/test/Z.jpg");
//			testLetters[i] = network->recognize(set.getSet(0));
		}
	}
	letters[0];
	//Mat image = imread("img/Bookman_Old_Style.jpg",0);  //dodaæ zabezpieczenia: co jak nie siê plik Ÿle otworzy
	//cut_letters(image);
*/	
//	Contour cont;
//	cont.getContour(image);
	QApplication a(argc, argv);
	App w;
	w.init();
	w.show();
	return a.exec();
}
