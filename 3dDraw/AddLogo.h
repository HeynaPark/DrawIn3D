#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class AddLogo {
public:
	int alpha;
	Mat logo;
	Point pos;
	String SetWindow = "Settings";


	void Draw(Mat plane);
	Point ChangePos(int x,int y, Mat plane);
	void ChangeAlpha(int &alpha);

	AddLogo();
};