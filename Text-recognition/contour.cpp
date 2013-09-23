#include "contour.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <qimage.h>
#include "LetterAnalyzer.h"
#include "app.h"
#include "Network.h"

using namespace cv;

int* Contour::extractContours(Mat& image,vector< vector<Point> > contours_poly, int& amount, Network* network)
{
    vector< vector<Point> > tmp_poly;
	//Sortowanie konturów wg x, a potem y 
	
	double gap = (contours_poly[0][0].y)/4;
	int n = contours_poly.size();
	do
	{
		for( int a = 0; a<n-1; a++ )
		{
		    if (((contours_poly[a][0].x) <= (contours_poly[a+1][0].x)) && ((contours_poly[a][0].y-gap) <= ((contours_poly[a+1][0].y))))
			{
				swap(contours_poly[a], contours_poly[a+1]);
			}
		}
		n = n - 1;
	} while (n>0);
	
	amount = contours_poly.size();
	int* recognizedLetters = new int[amount];
	int j = 0;

	for( int i = contours_poly.size() - 1; i >= 0; i-- )
	{
 
        Rect r = boundingRect( Mat(contours_poly[i]) );
                              
        Mat mask = Mat::zeros(image.size(), CV_8UC1);

        //Rysowanie maski
        drawContours(mask, contours_poly, i, Scalar(255), CV_FILLED);
 
        //wycinanie znaków
        Mat extractPic;
                
        image.copyTo(extractPic,mask);
        Mat resizedPic = extractPic(r);

        bitwise_not(resizedPic, resizedPic); //kontrast bo cv używa czarnego jako tła
        //zapisywanie
		/*stringstream file;
		
		file<<"letters/"<<(contours_poly.size()-i)<<".jpg";
        imwrite(file.str(),resizedPic);*/

		QImage qimg(resizedPic.data, resizedPic.cols, resizedPic.rows, resizedPic.step, QImage::Format_RGB888);
		qimg.save("test.jpg");
		qimg = LetterAnalyzer::crop(qimg);
		double* analyzed = LetterAnalyzer::parse(qimg);
		recognizedLetters[j++] = network->recognize(analyzed);
    }

	return recognizedLetters;
}



int* Contour::getContour(Mat img, int& amount, Network* network)
{
	//problem z plikami o malej rozdzielczości!

	//zamiana na czarno-bialy
	Size size(3,3); 
	GaussianBlur(img,img,size,0); //zamglenie
    adaptiveThreshold(img, img,255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,75,10); //progowanie
    bitwise_not(img, img); //kontrast bo cv uzywa czarnego jako tla
 
	//kopia
	Mat img2 = img.clone();
 
	//wycinamy zbedne tlo

	vector<Point> points;
    Mat_<uchar>::iterator it = img.begin<uchar>();
    Mat_<uchar>::iterator end = img.end<uchar>();
    for (; it != end; it++)
       if (*it)
          points.push_back(it.pos());

    //rysujemy linie pomocnicza do wycinania
    RotatedRect box = minAreaRect(Mat(points));
 
	double angle = box.angle;
       if (angle < -45.)
         angle += 90.;
         
    Point2f vertices[4];
    box.points(vertices);
    for(int i = 0; i < 4; ++i)
       line(img, vertices[i], vertices[(i + 1) % 4], Scalar(255, 0, 0), 1, CV_AA);
 
    Mat rot_mat = getRotationMatrix2D(box.center, angle, 1);
 
	//przeksztalcenie afiniczne
	Mat rotated;
	warpAffine(img2, rotated, rot_mat, img.size(), INTER_CUBIC);
 
	//wycinami srodek
	Size box_size = box.size;
	  if (box.angle < -45.)
		swap(box_size.width, box_size.height);
	Mat cropped;
	getRectSubPix(rotated, box_size, box.center, cropped);
	
	 Mat cropped2=cropped.clone();
	cvtColor(cropped2,cropped2,CV_GRAY2RGB);
 
	Mat cropped3 = cropped.clone();
	cvtColor(cropped3,cropped3,CV_GRAY2RGB);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
 
	//szukanie konturów
	findContours( cropped, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_TC89_KCOS, Point(0, 0) );
	
	///tworzenie z konturów wielokatów
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );
 
    
	//pobieranie konturów wielokatów
	for( int i = 0; i < contours.size(); i++ )
	{
		approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
	}
 
 
	//pobiera tylko najwazniejszych konturów
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
 
 
    //pobieranie granic prostokatów
    for(int i=0;i<validContours.size();i++){
            boundRect[i] = boundingRect( Mat(validContours[i]) );
    }
 
 
    //wyswietlanie konturów
	Scalar color = Scalar(0,255,0);
	for( int i = 0; i< validContours.size(); i++ )
		{
		if(boundRect[i].area()<10)
			continue;
		drawContours( cropped2, validContours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
		rectangle( cropped2, boundRect[i].tl(), boundRect[i].br(),color, 2, 8, 0 );
		}

	return extractContours(cropped3,validContours, amount, network);
}

