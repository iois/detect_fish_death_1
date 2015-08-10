#include "DetectFishDeath_1.h"
double cvCos(CvPoint a, CvPoint b)
{
	CvPoint p = { 0, 0 };
	return (Fish::Distance(a, p) + Fish::Distance(b, p) - Fish::Distance(a, b)) /
		(2 * sqrt(double(Fish::Distance(a, p)*Fish::Distance(b, p))));
}


DetectFishDeath_1::DetectFishDeath_1(int num_fishss, const vector<vector<Point> > &contours)
{
	num_fishss = num_fishss;

	for (int i = 0; i < num_fishss; ++i)
	{
		fishs.push_back(contours[i]);

		fish_Assigned.push_back(0);

		Moments m = moments(contours[i]);

		center_point.push_back(Point(m.m10 / m.m00, m.m01 / m.m00));//计算鱼的中心点
	}
}


DetectFishDeath_1::~DetectFishDeath_1()
{
}



void DetectFishDeath_1::update_data(vector<vector<Point>> contours)
{
	for (int i = 0; i < num_fishs; ++i){
		fish_Assigned[i] = 0;
	}
	// 没有重叠的情况，目标数量==鱼条数
	if (contours.size() == num_fishs){
		for (int i = 0; i < num_fishs; ++i)
		{
			//Moments m = moments(contours[i]);
			//Point center = Point(m.m10 / m.m00, m.m01 / m.m00);

			Fish temp_fish = get_fish_by_contour(contours[i]);

			//todo:
			//目标身份分配
			int index = FindIndex(center_point, temp_fish.center);

			fishs[index] = temp_fish;
		}
	}
}

vector<int> DetectFishDeath_1::get_is_tail_position()//返回尾巴的位置，1左2右
{
	vector<int> fish_tail_position;
	for (int i = 0; i < num_fishs; ++i)
	{
		CvPoint tmp1 = { fishs[i].center.x - fishs[i].head.x, fishs[i].center.y - fishs[i].head.y };	//Í·-ÖØÐÄ
		CvPoint tmp2 = { fishs[i].tail.x - fishs[i].center.x, fishs[i].tail.y - fishs[i].center.y };	//ÖØÐÄ-Î²
		double cos = cvCos(tmp1, tmp2);

		if (cos > 0.99)
		{
			if ((fishs[i].center.y - fishs[i].head.y)*(fishs[i].tail.x - fishs[i].center.x) + 
				(fishs[i].head.x - fishs[i].center.x)*(fishs[i].tail.y - fishs[i].center.y) > 0){

				fish_tail_position.push_back(1);
			}
			else{
				fish_tail_position.push_back(2);
			}
		}
		else{
			fish_tail_position.push_back(0);
		}
	}
	return fish_tail_position;
}