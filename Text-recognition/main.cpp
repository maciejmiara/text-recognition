#include "app.h"
#include "LetterAnalyzer.h"
//#include "Kohonen.h"
#include "Network.h"
#include <QtWidgets/QApplication>
#include <qimage.h>
#include <qimagereader.h>
#include <qimagewriter.h>
#include <qfile.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <Windows.h>

using namespace cv;
using namespace std;

class comparator{
public:
    bool operator()(vector<Point> c1,vector<Point>c2)
	{
		return boundingRect( Mat(c1)).x<boundingRect( Mat(c2)).x;
	}
};

void extractContours(Mat& image,vector< vector<Point> > contours_poly)
{
 
	//Sortowanie konturów od lewej do prawej wg x
	sort(contours_poly.begin(),contours_poly.end(),comparator());
 
	for( int i = 0; i< contours_poly.size(); i++ )
	{
 
        Rect r = boundingRect( Mat(contours_poly[i]) );
                              
        Mat mask = Mat::zeros(image.size(), CV_8UC1);

        //Rysowanie maski
        drawContours(mask, contours_poly, i, Scalar(255), CV_FILLED);
 
        //Wyszukiwanie liter z³o¿onych z dwóch znaków, np. "i"
		if(i+1<contours_poly.size())
		{
            Rect r2 = boundingRect( Mat(contours_poly[i+1]) );
            if(abs(r2.x-r.x)<10)
			{
                drawContours(mask, contours_poly, i+1, Scalar(255), CV_FILLED);
                i++;
                int minX = min(r.x,r2.x);
                int minY = min(r.y,r2.y);
                int maxX =  max(r.x+r.width,r2.x+r2.width);
                int maxY = max(r.y+r.height,r2.y+r2.height);
                r = Rect(minX,minY,maxX - minX,maxY-minY);
			}
        }

        //wycinanie znaków
        Mat extractPic;
                
        image.copyTo(extractPic,mask);
        Mat resizedPic = extractPic(r);
       
        bitwise_not(resizedPic, resizedPic); //kontrast bo cv u¿ywa czarnego jako t³a
        //zapisywanie
		stringstream file;
		
		char ch = (char)(i+65); 
        file<<"letters/"<<ch<<5<<".jpg";
        imwrite(file.str(),resizedPic);

 
    }
}

void image()
{
	
	//problem z plikami o ma³ej rozdzielczoœci!


	Mat img = imread("img/Bookman_Old_Style.jpg",0);  //dodaæ zabezpieczenia: co jak nie siê plik Ÿle otworzy

	//zamiana na czarno-bia³y
	Size size(3,3); 
	GaussianBlur(img,img,size,0); //zamglenie
    adaptiveThreshold(img, img,255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,75,10); //progowanie
    bitwise_not(img, img); //kontrast bo cv u¿ywa czarnego jako t³a
 
	//kopia
	Mat img2 = img.clone();
 
	//wycinamy zbêdne t³o

	vector<Point> points;
    Mat_<uchar>::iterator it = img.begin<uchar>();
    Mat_<uchar>::iterator end = img.end<uchar>();
    for (; it != end; it++)
       if (*it)
          points.push_back(it.pos());

    //rysujemy linie pomocnicz¹ do wycinania
    RotatedRect box = minAreaRect(Mat(points));
 
	double angle = box.angle;
       if (angle < -45.)
         angle += 90.;
         
    Point2f vertices[4];
    box.points(vertices);
    for(int i = 0; i < 4; ++i)
       line(img, vertices[i], vertices[(i + 1) % 4], Scalar(255, 0, 0), 1, CV_AA);
 
    Mat rot_mat = getRotationMatrix2D(box.center, angle, 1);
 
	//przekszta³cenie afiniczne
	Mat rotated;
	warpAffine(img2, rotated, rot_mat, img.size(), INTER_CUBIC);
 
	//wycinami œrodek
	Size box_size = box.size;
	  if (box.angle < -45.)
		swap(box_size.width, box_size.height);
	Mat cropped;
	getRectSubPix(rotated, box_size, box.center, cropped);
	//imshow("Wynik", cropped);
	
	 Mat cropped2=cropped.clone();
	cvtColor(cropped2,cropped2,CV_GRAY2RGB);
 
	Mat cropped3 = cropped.clone();
	cvtColor(cropped3,cropped3,CV_GRAY2RGB);

	//imshow("Wynik", cropped3);
	
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
 
	//szukanie konturów
	findContours( cropped, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	
	///tworzenie z konturów wielok¹tów
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );
 
 
	//pobieranie konturów wielok¹tów
	for( int i = 0; i < contours.size(); i++ )
	{
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
	}
 
 
	//pobiera tylko najwa¿niejszych konturów
	vector<vector<Point> > validContours;
    for (int i=0;i<contours_poly.size();i++)
	{
               
        Rect r = boundingRect(Mat(contours_poly[i]));
        if (r.area()<10)
			continue;
        bool inside = false;
        for(int j=0;j<contours_poly.size();j++)
		{
                if (j==i)
					continue;
                       
                Rect r2 = boundingRect(Mat(contours_poly[j]));
                if (r2.area()<10||r2.area()<r.area())
					continue;

                if(r.x>r2.x && r.x+r.width<r2.x+r2.width && r.y>r2.y && r.y+r.height< r2.y+r2.height)
				{
					inside = true;
                }
        }
        if(inside)continue;
        validContours.push_back(contours_poly[i]);
    }
 
 
    //pobieranie granic prostok¹tów
    for(int i=0;i<validContours.size();i++){
            boundRect[i] = boundingRect( Mat(validContours[i]) );
    }
 
 
    //wyœwietlanie konturów
	Scalar color = Scalar(0,255,0);
	for( int i = 0; i< validContours.size(); i++ )
		{
		if(boundRect[i].area()<10)
			continue;
		drawContours( cropped2, validContours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
		rectangle( cropped2, boundRect[i].tl(), boundRect[i].br(),color, 2, 8, 0 );
		}
 
	extractContours(cropped3,validContours);
	imshow("title",cropped2);

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

	//image();
	QImage img = QImage();

	TrainingSet set = TrainingSet();
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
				//j++;
				file.write("\n");
			}
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
	}

	file.close();

/*	TrainingSet trainingSets[4];
	trainingSets[0] = set;
	trainingSets[1] = set2;
	trainingSets[2] = set3;
	trainingSets[3] = set4;*/

	//Kohonen network = Kohonen();
/*	network.init();*/
//	network.learn(&set);
	//network.learn(&set2);
/*	network.learn(&set3);
	network.learn(&set4);*/

	Network* network = new Network();
	network->learn(78, &set);
	double normalizationFactor = 0.0;
	if (img.load("training-sets/J3.jpg"))
	{
		QImage cropped = LetterAnalyzer::crop(img);
		set.insertSet(0, LetterAnalyzer::parse(cropped));
		cropped.save("training-sets/test/Z.jpg");
		int letter = network->recognize(set.getSet(0));
	}
	QApplication a(argc, argv);
	App w;
	w.show();
	return a.exec();
}
