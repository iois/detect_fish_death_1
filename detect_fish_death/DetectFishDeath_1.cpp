#include "DetectFishDeath_1.h"


DetectFishDeath_1::DetectFishDeath_1()
{
	num_fishs = _num_fish;

	for (int i = 0; i < num_fishs; ++i)
	{
		vector <Point> tra(15, { 0, 0 });

		Moments m = moments(contours[i]);

		center_point.push_back(Point(m.m10 / m.m00, m.m01 / m.m00));//����������ĵ�

		contour_area.push_back(contourArea(contours[i]));//�����ⲿ�������������

		fish_Assigned.push_back(0);

		nearest_point.push_back(0);
	}
}


DetectFishDeath_1::~DetectFishDeath_1()
{
}
