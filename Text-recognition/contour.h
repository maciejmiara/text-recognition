#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;
using namespace std;

class Contour
{
public:
	void extractContours(Mat& image,vector< vector<Point> > contours_poly);
	void getContour(Mat img);
};
