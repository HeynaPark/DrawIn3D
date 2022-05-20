#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudafilters.hpp>
#include <opencv2/cudaimgproc.hpp>

using namespace cv;
using namespace std;


int num = 20;
Mat h[20] = {};					//Homograpy Matrix	(3d -> 2d)		
Mat h_T[20] = {};				//Homograpy Matrix	(2d -> 3d)
double wX[4] = {};
double wY[4] = {};
double pX[20][4] = { {},{},{},{} };		// dsc 개수만큼 배열 생성
double pY[20][4] = { {},{},{},{} };
double h11, h12, h13, h21, h22, h23, h31, h32;		//homogeneous matrix의 요소를 담는 변수
double th11, th12, th13, th21, th22, th23, th31, th32;



class TestProgram {
public:

	int color_c1 = 0;
	int color_c2 = 0;
	int color_t = 6;
	int color_l = 4;

	int line_type = 1;			// 1.solid	2.dotted(1)	3.dotted(2)
	int circle_type = 0;		// 1.spin	2.ring	3.dotted	4.solid(fill)
	int thick = 2;
	int r = 54, g = 173, b = 255;
	int max_alpha = 100;

	void CreateTrackBar();
};



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
	double alpha_ = 0.8;
	double alpha_arr_ = 0.8;
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
	int mAlpha_max_;
	int r, g, b;
	double abs_value;
	double abs_arr;
	double mAlpha_;

	int mTime_ani_ = 30;
	enum COLOR {
		CUSTOM, RED, ORANGE, YELLOW, GREEN, BLUE, CYAN, BLACK,
		GOLD, OLIVE, AQUAMARIN, SKYBLUE, VYOLET, PINK, MINT, BRIKRED, BROUN
	};
	/*int mColor_;
	int mColor_spin;*/
	COLOR mColor_;
	COLOR mColor_spin_;

	void SetColor(COLOR c, Scalar* color);
	void Appear(int t) { mAlpha_ += mAlpha_max_ / t; } //percent로 변환
	void Appear() { mAlpha_ = mAlpha_max_ * 255.0 / 100.0; }
	void Disppear(int t) { mAlpha_ -= (mAlpha_max_ * 255.0 / 100.0) / t; }


};

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

	void expandRad() { radius = mRadius_; }
	void expandRad(int t) { radius = ((1.0 - mRadius_start) * t / mTime_ani_ + mRadius_start) * mRadius_; }
	void expandRad(int t, int r) { mRadius_ = ((1.0 - mRadius_start) * t / mTime_ani_ + mRadius_start) * r; }
	void expandRad(int t, int time, int r) { mRadius_ = ((1.0 - mRadius_start) * t / time + mRadius_start) * r; }

	Circle();
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


	int mArrType_;
	double mThick_;
	enum class arrType { SOLID, DOTTED_LONG, DOTTED_SHORT };
	void Draw(Mat plane, int t);
	void CalcSetPos(Point p, Point* calcP);
	//void Drawline(int t);

	Arrow();
};



class Triangle :public Shape {

public:

	vector<Point> mPoint_;
	vector<Point> mWarpPoint_;

	void Draw(Mat plane);

	Triangle();
};


void showImg(string filename, Mat img, float factor);
void onMouse(int evt, int x, int y, int flags, void* param);
void CalcHomograpy(Point2d point, Point2d* h_point);
void calcHomograpy_T(Point2d point, Point2d* h_point);
void FindHomo(int n);


class Eventclass
{
public:
	void HandleEvent(int evt, int x, int y, int flags);
};

