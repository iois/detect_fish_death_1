#pragma once
#include<vector>
#include<cv.h>
using namespace std;
using namespace cv;
class DetectFishDeath_1
{
public:
	DetectFishDeath_1();
	~DetectFishDeath_1();

	void  update_data(vector<vector<Point>> contours);//
	vector<vector<int> > flags;

	double time_THRESH;
	double Angle_THRESH;
	double NEAREST_POINT_THRESH;

	vector<int> fish_Assigned;
	int num_fishs;

	int FindIndex(vector<Point> center_point, Point center);

	double get_nearest_ponit(Point center, const vector<Point> &contours);
};

