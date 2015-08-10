#pragma once
#pragma once
#include <cv.h>
#include <math.h>
#include <iostream>
#include <vector>


class Fish
{
public:
	CvPoint center;
	CvPoint head;
	CvPoint tail;

	Fish()
	{
		center.x = 0;
		center.y = 0;
		head.x = 0;
		head.y = 0;
		tail.x = 0;
		tail.y = 0;
	}

	Fish(const std::vector<cv::Point> &contours);

	Fish(const Fish &fish)
	{
		center.x = fish.center.x;
		center.y = fish.center.y;
		head.x = fish.head.x;
		head.y = fish.head.y;
		tail.x = fish.tail.x;
		tail.y = fish.tail.y;
	}

	~Fish();

	static int Distance(CvPoint a, CvPoint b)//¾àÀëµÄÆ½·½
	{
		int x = a.x > b.x ? (a.x - b.x) : (b.x - a.x);
		int y = a.y > b.y ? (a.y - b.y) : (b.y - a.y);
		return  (x * x + y * y);
	}
};


Fish get_fish_by_contour(const std::vector<cv::Point> &contour){
	Fish fish;
	if (contour.size() <= 0)
	{
		return;
	}
	int tempH = 0;
	int tempT = 0;
	fish.center.x = 0;
	fish.center.y = 0;
	fish.head.x = 0;
	fish.head.y = 0;
	fish.tail.x = 0;
	fish.tail.y = 0;
	CvPoint* p = NULL;
	CvPoint* p2 = NULL;
	int Pnum = 0;

	cv::Moments m = cv::moments(contour);

	fish.center.x = m.m10 / m.m00;
	fish.center.y = m.m01 / m.m00;

	for (int i = 0; i < contour.size(); ++i)
	{
		cv::Point p = contour[i];
		int temp = Fish::Distance(fish.center, p);
		if (temp > tempT)
		{
			tempT = temp;
			fish.tail.x = p.x;
			fish.tail.y = p.y;
		}
	}

	for (int i = 0; i < contour.size(); ++i)
	{
		cv::Point p = contour[i];
		int temp = Fish::Distance(fish.center, p);
		if ((Fish::Distance(p, fish.tail) >= (Fish::Distance(fish.center, p) + Fish::Distance(fish.center, fish.tail))) && (temp > tempH))
		{
			tempH = temp;
			fish.head.x = p.x;
			fish.head.y = p.y;
		}
	}
	return fish;
}