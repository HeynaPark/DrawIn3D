#include "AddLogo.h"

void AddLogo::Draw(Mat plane)
{
	Mat logo_src = imread("D:/test/4d.png", IMREAD_COLOR);
	logo = Mat::zeros(plane.size(), plane.type());
	Rect roi = Rect(pos, logo_src.size());

	logo = logo_src(roi);

	Mat temp, result;

	plane.copyTo(temp);


	namedWindow(SetWindow, WINDOW_AUTOSIZE);
	//imshow("logo add.", result);
	imshow("logo", logo);
	waitKey(0);
}

Point AddLogo::ChangePos(int x, int y, Mat plane)
{
	createTrackbar("Pos x", SetWindow, &x, plane.cols);
	createTrackbar("Pos y", SetWindow, &y, plane.rows);

	return Point(x, y);
}

void AddLogo::ChangeAlpha(int& alpha)
{

}

AddLogo::AddLogo()
{
	alpha = 70;
	pos = Point(100, 100);
}
