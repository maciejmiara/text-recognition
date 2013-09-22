#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>   

using namespace cv;
using namespace std;

class Contour
{
public:
	void extractContours(Mat& image,vector< vector<Point> > contours_poly);
	void getContour(Mat img);
};
