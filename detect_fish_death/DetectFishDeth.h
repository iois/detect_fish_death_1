#include <vector>
#include "opencv2/opencv.hpp"
#include <list>
#include <math.h>
#include <stdlib.h> 

using namespace std;
using namespace cv;

double sigmoid(double x);

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
			relative_poz.push_back((border_position - center_point[i].x) / MAX_POZ);
			//cout << "border_position:" << border_position << "center_point[i].x: " << center_point[i].x << endl;
		}
		return relative_poz;
	};    //�õ��㵽�߽�ľ���
	vector<double> get_speed();
	vector<double> get_diff_area();

	vector<double> get_prob_death(const vector<vector<Point> > &contours);


private:

	void  update_data(vector<vector<Point>> contours);//

	int FindIndex(vector<Point> center_point, Point center);
	vector<int> fish_Assigned;

	vector <Point>  center_point;
	vector <double>  contour_area;
	int num_fish;
	vector<vector<Point> > trajectory;

	double border_position;
	double avg_area;

	static int    SIZE_FRAME;
	static  double MAX_SPEED;
	static  double MAX_AREA;
	static  double MAX_POZ;

	static double theta1;
	static double theta2;
	static double theta3;

	static double alpha1;
	static double alpha2;
	static double alpha3;
};

