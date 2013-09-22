#pragma once

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "Network.h"
#include <math.h>   

using namespace cv;
using namespace std;

class Contour
{
public:
	int* extractContours(Mat& image,vector< vector<Point> > contours_poly, int& amount, Network* network);
	int* getContour(Mat img, int& amount, Network* network);
};
