#include "app.h"
#include "LetterCropper.h"
#include <QtWidgets/QApplication>
#include <qimage.h>
#include <qimagereader.h>
#include <qimagewriter.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <Windows.h>

using namespace cv;
using namespace std;

void image()
{
	//na razie pracuje na linijce tekstu, z którego wyci¹gam litery
	//potem bêdê wyci¹gaæ linijki z tekstu
	//rónie¿ na razie na sztywno jest ustawiony jeden plik

	Mat img = imread("img/image2.jpg",0);

	//zamiana na czarno-bia³y
	Size size(3,3); //dostosowaæ skalê do wielkoœci pliku? 
	GaussianBlur(img,img,size,0); //zamglenie
    adaptiveThreshold(img, img,255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,75,10); //progowanie
    //bitwise_not(img, img); //kontrast bo cv u¿ywa czarnego jako t³a
 
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
	imshow("Wynik", cropped);
	
 
	
    waitKey(0);
}

int main(int argc, char *argv[])
{
	image();
	QImage img = QImage();
	if (img.load("training-sets/Y2.jpg"))
	{
		QImage cropped = LetterCropper::crop(img);
		cropped.save("training-sets/testY2.jpg");
	}
	QApplication a(argc, argv);
	App w;
	w.show();
	return a.exec();
}

