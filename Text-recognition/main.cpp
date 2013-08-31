#include "app.h"
#include <QtWidgets/QApplication>

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
 
	//Sortowanie kontur�w od lewej do prawej wg x
	sort(contours_poly.begin(),contours_poly.end(),comparator());
 
	for( int i = 0; i< contours_poly.size(); i++ )
	{
 
        Rect r = boundingRect( Mat(contours_poly[i]) );
                              
        Mat mask = Mat::zeros(image.size(), CV_8UC1);

        //Rysowanie maski
        drawContours(mask, contours_poly, i, Scalar(255), CV_FILLED);
 
        //Wyszukiwanie liter z�o�onych z dw�ch znak�w, np. "i"
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

        //wycinanie znak�w
        Mat extractPic;
                
        image.copyTo(extractPic,mask);
        Mat resizedPic = extractPic(r);
       
        Mat image=resizedPic.clone();
 
        //zapisywanie
		stringstream file;
        file<<"letters/"<<i<<".jpg";
        imwrite(file.str(),resizedPic);

 
    }
}

void image()
{
	
	//problem z plikami o ma�ej rozdzielczo�ci!


	Mat img = imread("img/image3.jpg",0);  //doda� zabezpieczenia: co jak nie si� plik �le otworzy

	//zamiana na czarno-bia�y
	Size size(3,3); 
	GaussianBlur(img,img,size,0); //zamglenie
    adaptiveThreshold(img, img,255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,75,10); //progowanie
    bitwise_not(img, img); //kontrast bo cv u�ywa czarnego jako t�a
 
	//kopia
	Mat img2 = img.clone();
 
	//wycinamy zb�dne t�o

	vector<Point> points;
    Mat_<uchar>::iterator it = img.begin<uchar>();
    Mat_<uchar>::iterator end = img.end<uchar>();
    for (; it != end; it++)
       if (*it)
          points.push_back(it.pos());

    //rysujemy linie pomocnicz� do wycinania
    RotatedRect box = minAreaRect(Mat(points));
 
	double angle = box.angle;
       if (angle < -45.)
         angle += 90.;
         
    Point2f vertices[4];
    box.points(vertices);
    for(int i = 0; i < 4; ++i)
       line(img, vertices[i], vertices[(i + 1) % 4], Scalar(255, 0, 0), 1, CV_AA);
 
    Mat rot_mat = getRotationMatrix2D(box.center, angle, 1);
 
	//przekszta�cenie afiniczne
	Mat rotated;
	warpAffine(img2, rotated, rot_mat, img.size(), INTER_CUBIC);
 
	//wycinami �rodek
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
 
	//szukanie kontur�w
	findContours( cropped, contours, hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	
	///tworzenie z kontur�w wielok�t�w
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );
 
 
	//pobieranie kontur�w wielok�t�w
	for( int i = 0; i < contours.size(); i++ )
	{
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
	}
 
 
	//pobiera tylko najwa�niejszych kontur�w
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
 
 
    //pobieranie granic prostok�t�w
    for(int i=0;i<validContours.size();i++){
            boundRect[i] = boundingRect( Mat(validContours[i]) );
    }
 
 
    //wy�wirtlanie kontur�w
	Scalar color = Scalar(0,255,0);
	for( int i = 0; i< validContours.size(); i++ )
		{
		if(boundRect[i].area()<10)
			continue;
		drawContours( cropped2, validContours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
		rectangle( cropped2, boundRect[i].tl(), boundRect[i].br(),color, 2, 8, 0 );
		}
 
	extractContours(cropped3,validContours);


}

int main(int argc, char *argv[])
{
	image();
	QApplication a(argc, argv);
	App w;
	w.show();
	return a.exec();
}
