#include <iostream>
#include <time.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <fstream>
#include "parseJson.h"
#include "3dDraw.h"
#include "drawing.h"

#pragma comment(lib, "jsoncpp.lib")

#define FILE_NAME "1/000100.jpg"	

using namespace cv;
using namespace std;


// PATH
string pts_path = "1.pts";		//pts file
String path("1/*.jpg");
vector<String> str;


// H Matrix 
int frame_num = str.size();
int num = 20;
Mat h[20] = {};					//Homograpy Matrix	(3d -> 2d)		
Mat h_T[20] = {};				//Homograpy Matrix	(2d -> 3d)
double wX[4] = {};
double wY[4] = {};
double pX[20][4] = { {},{},{},{} };		// dsc 개수만큼 배열 생성
double pY[20][4] = { {},{},{},{} };
double h11, h12, h13, h21, h22, h23, h31, h32;		//homogeneous matrix의 요소를 담는 변수
double th11, th12, th13, th21, th22, th23, th31, th32;



// Image MAT
Mat frame;
Mat warp;		


vector<int> selTime(10);
// POINT
vector<Point2d> selPoint(10);
vector<Point2d> wPoint(10);
int fx, fy;				//첫번째 원의 point 
int fx2, fy2;			//두번째 원의 point



//color
class TestProgram {
public:

	int color_c1 = 0;
	int color_c2 = 0;
	int color_t = 6;
	int color_l = 4;

	int line_type = 1;			// 1.solid	2.dotted(1)	3.dotted(2)
	int circle_type = 0;		// 1.spin	2.ring	3.dotted	4.solid(fill)
	int thick = 2;
	int r =54, g=173, b=255;
	int max_alpha=100;
};

//parameter
float scale_view = 1;		//image scale

String SetWindow = "Settings";


enum eColors
{
	Custom,
	Red,
	Orange,
	Yellow,
	Green,
	Blue,
	Cyan,
	Black
};


class Animation
{
public:

	void Fadein(int start, int time);
	void Fadeout(int end, int time);
	void Swipe(int start, int end);
};

class Shape
{
private:


public:
	Scalar rgb;
	int mAbs_val_(int cm); 
	double abs_value;
	double abs_arr;
	double mAlpha_;
	int mAlpha_max_;
	int r, g, b;

	int mTime_ani_ = 30;
	enum COLOR { CUSTOM, RED, ORANGE, YELLOW, GREEN, BLUE, CYAN, BLACK,
		GOLD, OLIVE, AQUAMARIN, SKYBLUE, VYOLET, PINK, MINT, BRIKRED, BROUN
	};
	/*int mColor_;
	int mColor_spin;*/
	COLOR mColor_;
	COLOR mColor_spin_;

	void SetColor(COLOR c, Scalar* color);
	void Appear(int t) { mAlpha_ += mAlpha_max_ / t; } //percent로 변환
	void Appear() { mAlpha_ = mAlpha_max_ * 255.0 / 100.0; }
	void Disppear(int t) { mAlpha_ -= (mAlpha_max_ * 255.0/100.0) / t; }


};




int Shape::mAbs_val_(int cm)
{
	parseJson parse;
	parse.parsing_json();

	double max, min;


	for (int i = 0; i < 4; i++) {
		wX[i] = parse.wpointX[i];
		wY[i] = parse.wpointY[i];
	}

	max = wX[0];
	for (int i = 0; i < 3; i++)
	{
		if (wX[i] < wX[i + 1])
		{
			max = wX[i + 1];
		}
	}

	min = wX[0];
	for (int i = 0; i < 3; i++)
	{
		if (wX[i] > wX[i + 1])
		{
			min = wX[i + 1];
		}
	}

	double widPixel = max-min;
	double heiPixel = (abs(wY[2] - wY[1]) + abs(wY[3] - wY[0])) * 0.5;

	
	const double widReal = 13500;	//	(millimeter)
	const double heiReal = 3300;
	const double radiusReal = cm*10.0; // 0.5M

	double radius1, radius2;
	radius1 = double(radiusReal * widPixel) / widReal;
	radius2 = double(radiusReal * heiPixel) / heiReal;


	return radius1;
	//return  radius1 + radius2;
	
}
 

class Circle :public Shape {


	Scalar spin_rgb;
	double radius;
public:
	Point mCenter_;

	int mSpin_;
	double mRadius_;
	double mRadius_start;

	double mThickness_;

	bool mCir_Bigger_Flag_;

	enum cirType { CIRCLE_FILL, CIRCLE_RING, CIRCLE_DOTTED, CIRCLE_SPIN };
	cirType mCircle_type_;

	void Draw(Mat dst, int trans);
	
	void expandRad() { radius =  mRadius_; }
	void expandRad(int t) { radius = ((1.0 - mRadius_start) * t / mTime_ani_ + mRadius_start) * mRadius_; }
	void expandRad(int t, int r) { mRadius_ = ((1.0 - mRadius_start) * t / mTime_ani_ + mRadius_start) * r; }
	void expandRad(int t, int time, int r) { mRadius_ = ((1.0 - mRadius_start) * t / time + mRadius_start) * r; }
	
};



class Arrow :public Shape
{

private:
	
	double triEx, triEy;					//삼각형 끝 포인트
	double trip1x, trip1y, trip2x, trip2y;	//삼각형 나머지 두 포인트
	Point tip_start, tip_end;
	Point line_start, line_end;
public:
	double angle;

	Point mStart_, mEnd_;
//	Point mSetStart_, mSetEnd_;
	Point mAniEnd_;
	bool mSetStart_Flag_, mSetEnd_Flag_;
	bool mLinedraw_Flag_;
	

	double mThick_;
	enum class arrType { SOLID, DOTTED_LONG, DOTTED_SHORT };
	int mArrType_;
	void Draw(Mat plane, int t);
	void CalcSetPos(Point p, Point* calcP);
	//void Drawline(int t);
};



class Triangle :public Shape {

public:

	vector<Point> mPoint_;
	vector<Point> mWarpPoint_;

	void Draw(Mat plane);

};





void showImg(string filename, Mat img, float factor);
void onMouse(int evt, int x, int y, int flags, void* param);
void calcHomograpy(Point2d point, Point2d* h_point);
void calcHomograpy_T(Point2d point, Point2d* h_point);
void findHomo(int n);


class Eventclass
{
public:
	void HandleEvent(int evt, int x, int y, int flags);
};


// Object Make
Arrow arr1, arr2;
Circle cir1, cir2, cir3, cir4, cir5;

Triangle tri;
Point2d ani, ani2;
Shape sh;
TestProgram test;

vector<Arrow> _arrow;
vector<Circle> _circle;
vector<Triangle> _triangle;




//Mat mask(img.size(), CV_8UC1);



Mat img = imread("1/000100.jpg");

void generateMask(cuda::GpuMat& mask)
{
	int vfx = img.cols / 2, vfy = img.rows / 2;

	int percent = 70;
	double ratio = (percent / 100.0) / 2;
	int r1 = img.cols* ratio, r2 = img.rows* ratio;
	
	int col = 1;
	int row = 1;

	col = 1920;
	row = 1080;
	int blur_size = (col + row) / 2;
	Mat kernel_X = getGaussianKernel(col * 2, r1*2).rowRange(col - vfx, col * 2 - vfx);
	Mat kernel_Y = getGaussianKernel(row * 2, r2*2).rowRange(row - vfy, row * 2 - vfy);

	Mat kernel_X_transpose;
	transpose(kernel_X, kernel_X_transpose);
	Mat kernel = kernel_Y * kernel_X_transpose;

	double min1, max1;
	minMaxLoc(kernel, &min1, &max1);
	kernel = kernel / max1;
	kernel = kernel * 255;
	kernel.convertTo(kernel, CV_8UC1);


	cuda::GpuMat temp_c1 = cuda::GpuMat(img.size(), CV_8UC1);
	temp_c1.upload(kernel);

	cuda::GpuMat temp = cuda::GpuMat(img.size(), CV_8UC3);
	vector<cuda::GpuMat> bl_ch(3);
	cuda::split(temp, bl_ch);
	bl_ch[0] = temp_c1;
	bl_ch[1] = temp_c1;
	bl_ch[2] = temp_c1;
	cuda::merge(bl_ch, temp);


	mask = temp;

}

void initialize()
{


}

//
Scalar rgb_add, rgb_sub;
Scalar img_rgb;
double trans = 0.5;


void Shape::SetColor(COLOR c, Scalar* color) {

	int alpha = mAlpha_;
	int alpha_max = mAlpha_max_;
	r = test.r;
	g = test.g;
	b = test.b;


	switch (c)
	{
	case CUSTOM:
		//rgb = Scalar((255 - b) * alpha / alpha_max, (255 - g) * alpha / alpha_max, (255 - r) * alpha / alpha_max);				//custom
		rgb = Scalar((255 - b) / 255.0 * alpha, (255 - g) / 255.0 * alpha, (255 - r) / 255.0 * alpha);
		break;
	case RED:
		r = 255, g = 5, b = 3;
		break;
	case ORANGE:
		r = 255, g = 150, b = 50;
		break;
	case YELLOW:
		r = 255, g = 250, b = 50;
		break;
	case GREEN:
		r = 55, g = 250, b = 50;
		break;
	case BLUE:
		r = 55, g = 50, b = 250;
		break;
	case CYAN:
		r = 0, g = 255, b = 255;
		break;
	case BLACK:
		r = 255, g = 255, b = 255;
		break;
	case AQUAMARIN:
		r = 127, g = 255, b = 212;
		break;
	case GOLD:
		r = 255, g = 215, b = 0;
		break;
	case OLIVE:
		r = 85, g = 107, b = 47;
		break;
	case SKYBLUE:
		r = 30, g = 144, b = 255;
		break;
	case MINT:
		r = 255, g = 0, b = 0;
		break;
	case VYOLET:
		r = 152, g = 85, b = 211;
		break;
	case PINK:
		r = 255, g = 182, b = 203;
		break;
	}
	
	 rgb = Scalar(b,  g,  r);

	*color = rgb;
}

Mat roi, img_warp, frame_end, frame_start;
Mat p_img;


int n = 3;
Circle* pCir = new Circle[n];
Circle* p ;


int main(int ac, char** av)
{


	glob(path, str, false);

	selTime[0] = 0; // 첫 프레임
	selTime[1] = str.size() - 1;	// 마지막 프레임

	int frame1 = 0;
	int frame2 = selTime[1];

	//frame for draw
	frame_start = imread(str[frame1]);
	frame_end = imread(str[frame2]);

	if (str.size() == 0)
		cout << "image non exist. \n" << endl;




	img = imread(str[frame1]);
	Mat draw ;		//2d draw
	Mat draw_n;
	Mat drawimg = Mat::zeros(img.size(), img.type());
	warp  ;
	Mat result ;
	
	int blur_size = 2;

	
	clock_t start, end, start_1, end_1, start_2, end_2, start_3, end_3;


	selPoint[0] = Point(1248, 743);	//cir1
	selPoint[1] = Point(888, 848);	//cir2
	selPoint[2] = Point(1242, 543);	//arr1.s 
	selPoint[3] = Point(520.0, 596.0);	//arr1.e			   
	selPoint[4] = Point(519, 593);	//arr2.s
	selPoint[5] = Point(785, 644);	//arr2.e
	selPoint[6] = Point(1243, 544);
	selPoint[7] = Point(715, 773);	//tri
	selPoint[8] = Point(1261, 499);	//goal area 1
	selPoint[9] = Point(1339, 536);	//goal area 2



	namedWindow(SetWindow, WINDOW_AUTOSIZE);
	//createTrackbar("radius", SetWindow, &radius, 50);
	createTrackbar("C1 Color", SetWindow,  &test.color_c1, 7);
	createTrackbar("C2 Color", SetWindow, &test.color_c2, 7);
	createTrackbar("Line Color", SetWindow, &test.color_l, 7);
	createTrackbar("Tri Color", SetWindow, &test.color_t, 7);
	createTrackbar("R", SetWindow, &test.r, 255);
	createTrackbar("G", SetWindow, &test.g, 255);
	createTrackbar("B", SetWindow, &test.b, 255);
	createTrackbar("Circle/two color/Fill/null", SetWindow, &test.circle_type, 4);
	createTrackbar("Line/Arrow/null", SetWindow, &test.line_type, 2);
	createTrackbar("Transparency", SetWindow, &test.max_alpha, 255);


	cout << "str " << str.size() << endl;


	//find warp point 
	findHomo(frame1);
	for (int i = 0; i < 10; i++)
		calcHomograpy(selPoint[i], &wPoint[i]);

	//findHomo(str.size()-1);


	cout << "===============Select Mode==============" << endl << endl;
	cout << "1. Circle and Arrow (input 2 points) " << endl;
	cout << "2. 2 Circle , 1 Arrow (input 4 points) " << endl;
	cout << "3. 2 Circle , 1 Triangle (input 5 points) " << endl;
	int mode;
	//cin >> mode;
	mode = 4;




	while (1) {

		findHomo(0);

		////////////////////////////////////////
		/*	 Object Initialize	*/
		sh.abs_value = sh.mAbs_val_(100);
		sh.abs_arr = sh.mAbs_val_(100);

		/*	Circle1	*/
		cir1.mCenter_ = wPoint[0];
		cir1.mColor_ = Circle::CYAN;
		cir1.mColor_spin_ = Circle::SKYBLUE;
		cir1.mCircle_type_ = Circle::CIRCLE_SPIN;
		cir1.mRadius_ = sh.abs_value;
		cir1.mRadius_start = 0.5;
		cir1.mThickness_ = cir1.mRadius_ * 0.5;
		cir1.mAlpha_max_ = 100;

		/*	Circle2	*/
		cir2.mCenter_ = wPoint[1];
		cir2.mColor_ =Circle::YELLOW;
		cir2.mColor_spin_ = Circle::RED;
		cir2.mCircle_type_ = Circle::CIRCLE_SPIN;
		cir2.mRadius_ = sh.abs_value;
		cir2.mRadius_start = 0.5;
		cir2.mThickness_ = cir2.mRadius_ * 0.5;
		cir2.mAlpha_max_ = 100;

		/*	Circle3	*/
		cir3.mCenter_ = wPoint[2];
		cir3.mColor_ = Circle::OLIVE;
		cir3.mColor_spin_ = Circle::GREEN;
		cir3.mCircle_type_ = Circle::CIRCLE_SPIN;
		cir3.mRadius_ = sh.mAbs_val_(100);
		cir3.mRadius_start = 0.5;
		cir3.mThickness_ = cir2.mRadius_ * 0.5;
		cir3.mAlpha_max_ = 100;

		vector<Point> poly;
		poly.push_back(wPoint[5]);
		poly.push_back(Point(700, 500));
		poly.push_back(Point(300, 400));
	
		if (poly.size() < 3)
			cout << "polygon point false (pts<3)" << endl;
		
		/*	Triangle	*/
		for (int i = 0; i < poly.size(); i++)
			tri.mWarpPoint_.push_back(poly[i]);
		tri.mAlpha_max_ = 100;
		tri.mColor_ = Triangle::OLIVE;


		/*	Arrow1	*/
		double line_thick = 0.4;
		arr1.mStart_ = wPoint[0];
		arr1.mEnd_ = wPoint[1]; 
	/*	if (cir1.mCenter_ == arr1.mStart_)
			arr1.mSetStart_Flag_ = true;
		if (cir2.mCenter_ == arr1.mEnd_)
			arr1.mSetEnd_Flag_ = true;*/

		arr1.mArrType_ = 0;
		arr1.mThick_ = sh.abs_arr;
		arr1.mColor_ = Arrow::BLUE;
		arr1.mAlpha_max_ = 100;
	
		/*	Arrow2	*/
		arr2.mStart_ = wPoint[4];
		if (cir1.mCenter_ == arr2.mStart_)
			arr2.mSetStart_Flag_ = true;
		if (cir2.mCenter_ == arr2.mEnd_)
			arr2.mSetEnd_Flag_ = true;
		arr2.mEnd_ = wPoint[5];
		arr2.mArrType_ = 2;
		arr2.mThick_ = sh.abs_arr;
		arr2.mColor_ = Arrow::BLUE;
		arr2.mAlpha_max_ = 100;

	
		_circle.push_back(cir1);
		_circle.push_back(cir2);
		_circle.push_back(cir3);
		_arrow.push_back(arr1);
		_arrow.push_back(arr2);
		_triangle.push_back(tri);

		//////////////////////////////////////////////////////////



		/////////////////////////////////////////////
		/*		test		*/
		switch (mode) {
	
			break;

			case 4: 
			{
				while (1)
				{
					Animation ani;

					
					ani.Fadein(frame1, 1000);

					waitKey(0);

					ani.Swipe(frame1, frame2);

					ani.Fadeout(frame2, 1000);
				}
			}
				break;
		}



		waitKey(1);
	}

	delete[] pCir;

	return 0;
}

/// //////////////////////////////////////////

double alpha_ = 0.5;
double alpha_arr_ = 1;

void Animation::Fadein(int start, int time)
{
	Mat draw_black, warp;
	int time_frame = 33;
	int end_t = (time / time_frame);
	p_img = imread(str[start]);

	cuda::GpuMat mask;
	Mat black = Mat::zeros(p_img.size(), p_img.type());
	generateMask(mask);


	findHomo(start);
	Mat warp_add, warp_sub;
	//while (1) {
		for (int cnt = 0; cnt < end_t; cnt++)
		{
		
			Mat result = Mat::zeros(p_img.size(), p_img.type());
			Mat img_conv;

			draw_black = Mat::zeros(p_img.size(), p_img.type());
			warp_add = Mat::zeros(p_img.size(), p_img.type());
			warp_sub = Mat(p_img.size(), p_img.type());
			warp = Mat::zeros(p_img.size(), p_img.type());
		

			//animation
			_circle[0].expandRad();
			_circle[0].Appear(end_t);
			_circle[0].mCir_Bigger_Flag_ = true;
			_circle[1].expandRad(cnt);
			_circle[1].Appear(end_t);
			_circle[2].expandRad(cnt);
			_circle[2].Appear(end_t);
			_arrow[0].mLinedraw_Flag_ = true;
			_arrow[0].Appear(end_t);
			_arrow[1].mLinedraw_Flag_ = true;
			_arrow[1].Appear(end_t);
			_triangle[0].Appear(end_t);


			//draw
			Mat draw_white = Mat(p_img.size(), p_img.type(), Scalar::all(255));
			Mat warp_white = Mat(p_img.size(), p_img.type());

			_circle[0].Draw(draw_white, 255);
			_circle[1].Draw(draw_white, 255);
			_circle[2].Draw(draw_white, 255);


			_circle[0].mSpin_ += 3;
			_circle[1].mSpin_ += 3;
			_circle[2].mSpin_ += 3;


			warpPerspective(draw_white, warp_white, h_T[start], p_img.size(), 1, BORDER_CONSTANT, Scalar(255, 255, 255));
			subtract(p_img, warp_white, warp_sub);
			subtract(p_img, warp_sub * alpha_, result);

	
			setMouseCallback("warp_sub", onMouse);
			setMouseCallback("warp_add", onMouse);
			setMouseCallback("result", onMouse);


			_arrow[0].Draw(draw_black, cnt);
			_arrow[0].Draw(draw_white, cnt);
			_arrow[1].Draw(draw_black, cnt);
			_arrow[1].Draw(draw_white, cnt);


			warpPerspective(draw_black, warp, h_T[start], p_img.size(), 1, 0);
			warpPerspective(draw_white, warp_white, h_T[start], p_img.size(), 1, BORDER_CONSTANT, Scalar(255, 255, 255));

			subtract(result, warp_white, warp_sub);
			subtract(warp, result, warp_add);

			subtract(result, warp_sub, result);
			add(result, warp_add, result);


			imshow("result", result);


			cuda::GpuMat result_gpu;
			result_gpu.upload(result);


			cuda::GpuMat dst_gpu;
			cuda::GpuMat mask_inv = cuda::GpuMat(p_img.size(), CV_8UC3, Scalar::all(255));
	
	
			cuda::subtract(mask_inv, mask, mask_inv);
			cuda::subtract(result_gpu, mask_inv, dst_gpu);
			cuda::GpuMat vignet_gpu;

			dst_gpu.download(result);

	
			Mat mask_cpu;
			mask_inv.download(mask_cpu);
			//imshow("mask V ", mask_cpu);
	
			waitKey(1);

		}
		waitKey(1);
	//}
}


void Animation::Swipe(int start, int end)
{


	Mat draw_black,warp_add,warp_sub, warp, result;

	for (int idx = start; idx < end-1; idx++)			//
	{
		p_img = imread(str[idx]);
		Mat result = Mat::zeros(p_img.size(), p_img.type());
		Mat img_conv;

		draw_black = Mat::zeros(p_img.size(), p_img.type());
		warp_add = Mat::zeros(p_img.size(), p_img.type());
		warp_sub = Mat(p_img.size(), p_img.type());
		warp = Mat::zeros(p_img.size(), p_img.type());


		//animation
		_arrow[0].mLinedraw_Flag_ = false;
		_arrow[1].mLinedraw_Flag_ = false;


		//draw
		Mat draw_white = Mat(p_img.size(), p_img.type(), Scalar::all(255));
		Mat warp_white = Mat(p_img.size(), p_img.type());

		_circle[0].Draw(draw_white, 255);
		_circle[1].Draw(draw_white, 255);
		_circle[2].Draw(draw_white, 255);


		_circle[0].mSpin_ += 2;
		_circle[1].mSpin_ += 2;
		_circle[2].mSpin_ += 2;

		_triangle[0].Draw(draw_white);


		findHomo(idx);
		warpPerspective(draw_white, warp_white, h_T[idx], p_img.size(), 1, BORDER_CONSTANT, Scalar(255, 255, 255));


		subtract(p_img, warp_white, warp_sub);


		subtract(p_img, warp_sub * alpha_, result);


		_arrow[0].Draw(draw_black, end);
		_arrow[0].Draw(draw_white, end);
		_arrow[1].Draw(draw_black, end);
		_arrow[1].Draw(draw_white, end);


		warpPerspective(draw_black, warp, h_T[idx], p_img.size(), 1, 0);
		warpPerspective(draw_white, warp_white, h_T[idx], p_img.size(), 1, BORDER_CONSTANT, Scalar(255, 255, 255));

		subtract(result, warp_white, warp_sub);
		subtract(warp, result, warp_add);

		subtract(result, warp_sub , result);
		add(result, warp_add , result);

		_arrow[0].Draw(draw_black, end);
		_arrow[0].Draw(draw_white, end);
		_arrow[1].Draw(draw_black, end);
		_arrow[1].Draw(draw_white, end);

	
		setMouseCallback("warp_sub", onMouse);
		setMouseCallback("warp_add", onMouse);
		setMouseCallback("result", onMouse);

		Mat test_size;

		imshow("result", result);

		waitKey(100);

	}
	waitKey(0);
}

void Animation::Fadeout(int end, int time)
{

	Mat draw_black, warp_add, warp_sub, warp, result;
	int time_frame = 33;

	
	p_img = imread(str[end]);
	findHomo(end);
	for (int cnt = 0; cnt < time / time_frame; cnt++)
	{
		Mat result = Mat::zeros(p_img.size(), p_img.type());
		Mat img_conv;

		draw_black = Mat::zeros(p_img.size(), p_img.type());
		warp_add = Mat::zeros(p_img.size(), p_img.type());
		warp_sub = Mat(p_img.size(), p_img.type());
		warp = Mat::zeros(p_img.size(), p_img.type());


		//animation
		_circle[0].Disppear(time / time_frame);
		_circle[1].Disppear(time / time_frame);
		_circle[2].Disppear(time / time_frame);
		_arrow[0].mLinedraw_Flag_ = false;
		_arrow[0].Disppear(time / time_frame);
		_arrow[1].mLinedraw_Flag_ = false;
		_arrow[1].Disppear(time / time_frame);


		//draw
		Mat draw_white = Mat(p_img.size(), p_img.type(), Scalar::all(255));
		Mat warp_white = Mat(p_img.size(), p_img.type());
		_circle[0].Draw(draw_black, 0);
		_circle[0].Draw(draw_white, 255);
		_circle[1].Draw(draw_black, 0);
		_circle[1].Draw(draw_white, 255);
		_circle[2].Draw(draw_black, 0);
		_circle[2].Draw(draw_white, 255);


		_circle[0].mSpin_ += 4;
		_circle[1].mSpin_ += 4;
		_circle[2].mSpin_ += 4;


		_triangle[0].Draw(draw_black);
		_triangle[0].Draw(draw_white);

		_arrow[0].Draw(draw_black, cnt);
		_arrow[0].Draw(draw_white, cnt);
		_arrow[1].Draw(draw_black, cnt);
		_arrow[1].Draw(draw_white, cnt);



		warpPerspective(draw_black, warp, h_T[end], p_img.size(), 1, 0);
		warpPerspective(draw_white, warp_white, h_T[end], p_img.size(), 1, BORDER_CONSTANT, Scalar(255, 255, 255));


		subtract(p_img, warp_white, warp_sub);
		subtract(warp, p_img, warp_add);

		subtract(p_img, warp_sub * alpha_, result);
		add(result, warp_add * alpha_, result);

		

		setMouseCallback("warp_sub", onMouse);
		setMouseCallback("warp_add", onMouse);
		setMouseCallback("result", onMouse);

		imshow("result", result);



		waitKey(1);
	}
}


void findHomo(int n)
{

	parseJson parse;
	parse.parsing_json();

	for (int i = 0; i < 4; i++) {
		wX[i] = parse.wpointX[i]+560;
		wY[i] = parse.wpointY[i]+140 ;
	}

	for (int j = 0; j < str.size(); j++)
	{
		for (int i = 0; i < 4; i++) {
			pX[j][i] = parse.pointX[j][i];
			pY[j][i] = parse.pointY[j][i];
		}
	}


	vector<Point2d> pts_src, pts_dst;


	for (size_t i(0); i < 4; i++) {
		pts_src.push_back(Point2d(pX[n][i], pY[n][i]));
		pts_dst.push_back(Point2d(wX[i], wY[i]));
	}
	//img to plane
	h[n] = findHomography(pts_src, pts_dst, RANSAC);

	h11 = h[n].at<double>(0, 0);
	h12 = h[n].at<double>(0, 1);
	h13 = h[n].at<double>(0, 2);
	h21 = h[n].at<double>(1, 0);
	h22 = h[n].at<double>(1, 1);
	h23 = h[n].at<double>(1, 2);
	h31 = h[n].at<double>(2, 0);
	h32 = h[n].at<double>(2, 1);

	//plane to img
	h_T[n] = findHomography(pts_dst, pts_src, cv::RANSAC);

	th11 = h_T[n].at<double>(0, 0);
	th12 = h_T[n].at<double>(0, 1);
	th13 = h_T[n].at<double>(0, 2);
	th21 = h_T[n].at<double>(1, 0);
	th22 = h_T[n].at<double>(1, 1);
	th23 = h_T[n].at<double>(1, 2);
	th31 = h_T[n].at<double>(2, 0);
	th32 = h_T[n].at<double>(2, 1);

}


void calcHomograpy(Point2d point, Point2d* h_point)
{
	double dx = (h11 * point.x + h12 * point.y + h13) / (h31 * point.x + h32 * point.y + 1);
	double dy = (h21 * point.x + h22 * point.y + h23) / (h31 * point.x + h32 * point.y + 1);

	*h_point = Point2d(dx, dy);
}

void calcHomograpy_T(Point2d point, Point2d* h_point)
{
	double dx = (th11 * point.x + th12 * point.y + th13) / (th31 * point.x + th32 * point.y + 1);
	double dy = (th21 * point.x + th22 * point.y + th23) / (th31 * point.x + th32 * point.y + 1);

	*h_point = Point2d(dx, dy);
}


void showImg(string filename, Mat img, float factor)
{
	resize(img, img, Size(img.cols/2,img.rows/2), INTER_CUBIC);
	namedWindow(filename, 0);
	imshow(filename, img);
	//resizeWindow(filename, img.cols * factor, img.rows * factor);
}




void Circle::Draw(Mat plane, int trans)
{
	double rad;
	
	rad = radius;

	int beta = 30; //beta는 360의 약수, 점선의 간격을 설정

	SetColor(mColor_spin_, &spin_rgb);
	SetColor(mColor_, &rgb);


	Point center = mCenter_;
    int spin = mSpin_;
	
	double axes = rad;
	double hole = axes * (10 - 4) / 10.0;


	switch (mCircle_type_)
	{
	case CIRCLE_SPIN:
		ellipse(plane, Point(center), Size(axes, axes), 0, 0, 360, rgb, -1, LINE_AA);				//원
		ellipse(plane, Point(center), Size(axes, axes), spin, 0, 45, spin_rgb, -1, LINE_AA);
		ellipse(plane, Point(center), Size(axes+1, axes+1), 0, 0, 360, Scalar::all(trans), 1, LINE_AA);// 자르기
		ellipse(plane, Point(center), Size(hole, hole), 0, 0, 360, Scalar::all(trans), -1, LINE_AA);

		break;

	case CIRCLE_RING:
		ellipse(plane, Point(center), Size(axes, axes), 0, 0, 360, rgb, -1, LINE_AA);
		ellipse(plane, Point(center), Size(hole, hole), 0, 0, 360, Scalar(trans, trans, trans), -1, LINE_AA);
		break;

	case CIRCLE_DOTTED:
		ellipse(plane, Point(center), Size(axes, axes), 0, 0, 360, rgb, -1, LINE_AA);
		for (int i = 0; i < 360 / beta; i++)
		{
			ellipse(plane, Point(center), Size(axes + 2, axes + 2), spin + beta * i, 0, 9, Scalar(trans, trans, trans), -1, LINE_AA);
		
		}
		ellipse(plane, Point(center), Size(axes * 0.7, axes * 0.7), 0, 0, 360, Scalar(trans, trans, trans), -1, LINE_AA);
		break;

	case CIRCLE_FILL:
		ellipse(plane, Point(center), Size(axes, axes), 0, 0, 360, rgb, -1, LINE_AA);
		break;

	default:
		break;
	}
}


void Arrow::Draw(Mat plane, int t)
{
	double time = sh.mTime_ani_;
	double d = 0;

	Point aniPoint;
	Point tip_start, tip_end;
	Point line_start, line_end;


	static double p = 25.0;

	double thick = mThick_;
	double line_t=0.4;

	// drawing과 삼각형위치 계산을 위한 Angle 계산
	if (mStart_.x == mEnd_.x)
	{
		if (mEnd_.y > mStart_.y)
			angle = CV_PI / 2.0;
	
		else
			angle = CV_PI * (3.0 / 2.0);
	}
	else {
		angle = atan(double(abs(mEnd_.y - mStart_.y)) / double(abs(mEnd_.x - mStart_.x)));
		if (mEnd_.x > mStart_.x && mEnd_.y >= mStart_.y)
		{

		}
		else if (mStart_.x > mEnd_.x && mEnd_.y >= mStart_.y)
		{
			angle = CV_PI - angle;
		}
		else if (mStart_.x > mEnd_.x && mStart_.y > mEnd_.y)
		{
			angle = CV_PI + angle;
		}
		else if (mEnd_.x > mStart_.x && mStart_.y > mEnd_.y)
		{
			angle = 2 * CV_PI - angle;
		}
	}

	SetColor(mColor_, &rgb);



	if (mSetStart_Flag_)
	{
		line_start.x = mStart_.x + thick * 1.2 * cos(angle);
		line_start.y = mStart_.y + thick * 1.2 * sin(angle);
		tip_start.x = line_start.x + thick * (sqrt(3) / 2.0) * cos(angle);
		tip_start.y = line_start.y + thick * (sqrt(3) / 2.0) * sin(angle);
	}
	else
	{
		line_start.x = mStart_.x;
		line_start.y = mStart_.y;
		tip_start.x = line_start.x + thick * (sqrt(3) / 2.0) * cos(angle);
		tip_start.y = line_start.y + thick * (sqrt(3) / 2.0) * sin(angle);
	}

	if (mSetEnd_Flag_ == true && mLinedraw_Flag_ == true)
	{
		tip_end.x = mEnd_.x - thick * cos(angle);
		tip_end.y = mEnd_.y - thick * sin(angle);
		aniPoint.x = double((tip_start.x + (tip_end.x - tip_start.x) * ((1.0 - d) * pow((t + 1) / time, 2) + d)));
		aniPoint.y = double((tip_start.y + (tip_end.y - tip_start.y) * ((1.0 - d) * pow((t + 1) / time, 2) + d)));
		tip_end = aniPoint;
	}

	if (mSetEnd_Flag_ == true && mLinedraw_Flag_ == false)
	{
		tip_end.x = mEnd_.x - thick * cos(angle);
		tip_end.y = mEnd_.y - thick * sin(angle);
	}


	if (mSetEnd_Flag_ == false && mLinedraw_Flag_ == true)
	{
		aniPoint.x = double((tip_start.x + (mEnd_.x - tip_start.x) * ((1.0 - d) * pow((t + 1) / time, 2) + d)));
		aniPoint.y = double((tip_start.y + (mEnd_.y - tip_start.y) * ((1.0 - d) * pow((t + 1) / time, 2) + d)));
		tip_end = aniPoint;
	}

	if (mSetEnd_Flag_ == false && mLinedraw_Flag_ == false)
	{
		tip_end.x = mEnd_.x;
		tip_end.y = mEnd_.y;
	}
	line_end.x = tip_end.x - thick * (sqrt(3) / 2.0) * cos(angle);
	line_end.y = tip_end.y - thick * (sqrt(3) / 2.0) * sin(angle);



	Point pts[3];
	double len;
	len = sqrt(pow(line_end.x - line_start.x, 2) + pow(line_end.y - line_start.y, 2));

	int tip_angle = 30;
	triEx = tip_end.x;
	triEy = tip_end.y;
	trip1x = triEx - thick * cos(angle + tip_angle * M_PI / 180);
	trip1y = triEy - thick * sin(angle + tip_angle * M_PI / 180);
	trip2x = triEx - thick * cos(angle - tip_angle * M_PI / 180);
	trip2y = triEy - thick * sin(angle - tip_angle * M_PI / 180);
	pts[0] = Point(triEx, triEy);
	pts[1] = Point(trip1x, trip1y);
	pts[2] = Point(trip2x, trip2y);
	const Point* ppt[1] = { pts };
	int npt[] = { 3 };

	double dot = thick/1.5;

	switch (mArrType_)
	{
	case static_cast<int>(Arrow::arrType::SOLID):


		line(plane, line_start, line_end, rgb, thick*line_t, LINE_AA);
		fillPoly(plane, ppt, npt, 1, rgb, LINE_AA);
		break;


	case static_cast<int>(Arrow::arrType::DOTTED_SHORT):
		fillPoly(plane, ppt, npt, 1, rgb, LINE_AA);
		for (int i = 1; i < len / dot ; i++)
		{
			circle(plane, Point(line_start.x + (i * dot * cos(angle)), line_start.y + (i * dot * sin(angle))), round(thick / 5.0), rgb, -1, LINE_AA);
		}
		break;

	default:
		break;
	}
}

//draw 3d calib ROI 
void Triangle::Draw(Mat dst)
{
	SetColor(mColor_, &rgb);

	int pts_size;
	pts_size = mWarpPoint_.size();

	Point *pts = new Point[pts_size];

	for (int i = 0; i < pts_size; i++)
		pts[i] = mWarpPoint_[i];

	const Point* ppt[1] = { pts };
	int npt[] = { pts_size };

	fillPoly(dst, ppt, npt, 1, rgb, LINE_AA);

	delete [] pts;

}




void Eventclass::HandleEvent(int evt, int x, int y, int flags)
{

	if (evt == EVENT_LBUTTONDOWN)
	{

		fx = x;
		fy = y;
		cout << "\nx,y:" << x << "," << y << endl;

		cout << "flip: " << 3840 - x << "," << 2160 - y << endl;

	}

	if (evt == EVENT_RBUTTONDOWN)
	{
		fx2 = x;
		fy2 = y;
		cout << "\n2x,2y:" << x << "," << y << endl;
	}

}


void onMouse(int evt, int x, int y, int flags, void* param)
{
	Eventclass* p = (Eventclass*)param;
	p->HandleEvent(evt, x, y, flags);
}
