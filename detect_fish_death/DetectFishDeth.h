#include <vector>
#include "opencv2/opencv.hpp"
#include <list>
#include <math.h>
#include <stdlib.h> 

using namespace std;
using namespace cv;

const int    SIZE_FRAME = 15;
const double MAX_SPEED = 50;
const double MAX_AREA = 1000;
const double MAX_POZ = 600;

class DetectFishDeth
{
	//   ������Ƿ������������ж�
public:
	DetectFishDeth(const vector<vector<Point> > &contours, int border_position, int avg_area);
	~DetectFishDeth();
	vector<double> get_relativepoz()
	{
		vector<double> relative_poz;
		for (int i = 0; i < num_fish; ++i)
		{
			relative_poz.push_back(abs(border_position - center_point[i].x) / MAX_POZ);
		}
		return relative_poz;
	};    //�õ��㵽�߽�ľ���
	vector<double> get_speed();
	vector<double> get_diff_area();


	vector<double> get_prob_death(const vector<vector<Point> > &contours);


private:

	void  update_data(const vector<vector<Point> > &contours);//


	vector <Point>  center_point;
	vector <double>  contour_area;
	int num_fish;
	vector<vector<Point> > trajectory;

	double border_position;
	double avg_area;
};

DetectFishDeth::DetectFishDeth(const vector<vector<Point> > &contours, int bp, int _avg_area)
{
	num_fish = contours.size();

	border_position = bp;
	avg_area = _avg_area;

	for (int i = 0; i < num_fish; ++i)
	{
		vector <Point> tra(SIZE_FRAME, { 0, 0 });

		trajectory.push_back(tra);

		Moments m = moments(contours[i]);

		center_point.push_back(Point(m.m10 / m.m00, m.m01 / m.m00));//����������ĵ�

		contour_area.push_back(contourArea(contours[i]));//�����ⲿ�������������
	}
}
void DetectFishDeth::update_data(const vector<vector<Point> > &contours)
{
	for (int i = 0; i < num_fish; ++i)
	{
		Moments m = moments(contours[i]);
		Point center = Point(m.m10 / m.m00, m.m01 / m.m00);
		trajectory[i].insert(trajectory[i].cbegin(), center);
		trajectory[i].pop_back();
		center_point[i] = center;
		contour_area[i] = contourArea(contours[i]);
	}
}
vector<double> DetectFishDeth::get_prob_death(const vector<vector<Point> > &contours)
{
	this->update_data(contours);

	vector<double> prob;
	vector<double> x = get_relativepoz();   //λ��	
	vector<double> diffarea = get_diff_area();   //���
	vector<double> _speed = get_speed();   //�ٶ�

	vector<double> y1;
	vector<double> y2;
	vector<double> y3;

	for (int i = 0; i < num_fish; ++i)
	{
		y3.push_back(-log(_speed[i]));
		y2.push_back(-log(1 - diffarea[i]));
		y1.push_back(-log(x[i]));
	}
	double theta1 = 1, theta2 = 1, theta3 = 1;
	for (int i = 0; i < num_fish; ++i)
	{
		prob.push_back(tanh(theta1*y1[i] + theta2*y2[i] + theta3*y3[i]));
	}
	return prob;
}
vector<double> DetectFishDeth::get_speed()
{
	vector<double> speed;


	for (int i = 0; i < num_fish; ++i)
	{
		double x_sum = 0;
		double y_sum = 0;
		for (int j = 1; j < trajectory[i].size(); ++j)
		{
			x_sum += abs(trajectory[i][j].x - trajectory[i][j - 1].x);
			y_sum += abs(trajectory[i][j].y - trajectory[i][j - 1].y);
		}
		speed.push_back(sqrt(x_sum*x_sum + y_sum*y_sum) / MAX_SPEED);
	}
	return speed;
}

vector<double> DetectFishDeth::get_diff_area()
{
	//todo: �����ص������
	vector<double> diff_area;
	for (int i = 0; i < num_fish; ++i)
	{
		if (contour_area[i] > avg_area){
			diff_area.push_back((contour_area[i] - avg_area) / MAX_AREA);
		}
		else{
			diff_area.push_back(0.05);
		}
	}
	return diff_area;
}

DetectFishDeth::~DetectFishDeth()
{

}