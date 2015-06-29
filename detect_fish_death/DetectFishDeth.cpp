#include"DetectFishDeth.h"

int    DetectFishDeth::SIZE_FRAME=15;
double DetectFishDeth::MAX_SPEED=60;
double DetectFishDeth::MAX_AREA=1400;
double DetectFishDeth::MAX_POZ=640;

double DetectFishDeth::theta1=0.8;//relative position
double DetectFishDeth::theta2=0.7;
double DetectFishDeth::theta3=1.5;//speed

double DetectFishDeth::alpha1=2;//
double DetectFishDeth::alpha2=1;
double DetectFishDeth::alpha3=2;

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

		fish_Assigned.push_back(0);
	}
}

void DetectFishDeth::update_data(vector<vector<Point>> contours)
{
	for (int i = 0; i < num_fish; ++i){
		fish_Assigned[i] = 0;
	}
	// û���ص��������Ŀ������==������
	if (contours.size() == num_fish){
		for (int i = 0; i < num_fish; ++i)
		{
			Moments m = moments(contours[i]);
			Point center = Point(m.m10 / m.m00, m.m01 / m.m00);

			//todo:
			//Ŀ����ݷ���
			int index = FindIndex(center_point, center);

			trajectory[index].insert(trajectory[index].cbegin(), center);
			trajectory[index].pop_back();
			center_point[index] = center;
			contour_area[index] = contourArea(contours[i]);
			//cout << index<<": "<<contour_area[index] << endl;
		}
	}
	//�ص������
	else if (contours.size() < num_fish){

		sort(contours.begin(), contours.end(), [](vector<Point> a, vector<Point> b){return contourArea(a) > contourArea(b); });

		for (int j = 0; j < contours.size(); ++j){
			double n = round(contourArea(contours[j]) / avg_area);//�ص������м�����

			Moments m = moments(contours[j]);
			Point center = Point(m.m10 / m.m00, m.m01 / m.m00);

			for (int i = 0; i < n; ++i){
				int index = FindIndex(center_point, center);
				trajectory[index].insert(trajectory[index].cbegin(), center);
				trajectory[index].pop_back();
				center_point[index] = center;
				contour_area[index] = contourArea(contours[j]) / n;
			}
		}
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
	//double  = 1, theta2 = 1, theta3 = 1;
	for (int i = 0; i < num_fish; ++i)
	{
		y3.push_back(-log(_speed[i]*alpha3));
		y2.push_back(-log((1 - diffarea[i])*alpha2));
		y1.push_back(-log(x[i]*alpha1));
		//cout << "y3: " << y3[i] << endl;
		//cout << "y1:" << y1[i] << " y2:" << y2[i] << " y3:" << y3[i] << endl;
	}
	
	for (int i = 0; i < num_fish; ++i)
	{
		prob.push_back(sigmoid(theta1*y1[i] + theta2*y2[i] + theta3*y3[i]));
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
		double s=0;
		for (int j = 1; j < trajectory[i].size(); ++j)
		{
			x_sum = pow((trajectory[i][j].x - trajectory[i][j - 1].x),2);
			y_sum = pow((trajectory[i][j].y - trajectory[i][j - 1].y),2);
			s += sqrt(x_sum + y_sum);
		}
		//cout << "s: " << s << endl;
		if (s > MAX_SPEED){
			speed.push_back(1);
		}
		else{
			speed.push_back(s/ MAX_SPEED);
		}
	}
	return speed;
}

vector<double> DetectFishDeth::get_diff_area()
{
	//todo: �����ص������
	vector<double> diff_area;
	for (int i = 0; i < num_fish; ++i)
	{
		//if (contour_area[i] > avg_area){
			diff_area.push_back((contour_area[i] - avg_area) / MAX_AREA);
		//}
		//else{
		//	diff_area.push_back(0.05);
		//}
	}
	return diff_area;
}

DetectFishDeth::~DetectFishDeth()
{

}

int DetectFishDeth::FindIndex(vector<Point> center_point, Point center)
{
	int index = 0;
	double distence2 = 0;
	double max_distence = INT_MAX;
	for (int j = 0; j < num_fish; j++)
	{
		distence2 = (pow(center_point[j].x - center.x, 2) + pow(center_point[j].y - center.y, 2));

		if (distence2<max_distence && fish_Assigned[j] == 0)
		{
			index = j;
			max_distence = distence2;
		}
	}
	fish_Assigned[index] = 1;
	return index;
}

double sigmoid(double x){
	return 1 / (1 + exp(-x));
}