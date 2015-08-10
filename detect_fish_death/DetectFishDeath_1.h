#pragma once
#include<vector>
#include<cv.h>
#include"Fish.h"
using namespace std;
using namespace cv;


class DetectFishDeath_1
{
public:
	DetectFishDeath_1(int num_fishs, const vector<vector<Point> > &contours);
	~DetectFishDeath_1();

	void  update_data(vector<vector<Point>> contours);//input contours

	vector<int> get_is_tail_position();//·µ»ØÎ²°ÍµÄÎ»ÖÃ£¬1×ó2ÓÒ

private:

	vector<Fish>  fishs;
	vector<int> fish_Assigned;

	double time_THRESH;
	double Angle_THRESH;
	double NEAREST_POINT_THRESH;

	vector <Point>  center_point;

	int num_fishs;

	int FindIndex(vector<Point> center_point, Point center);

	double get_nearest_ponit(Point center, const vector<Point> &contours);
};

