#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
class DetectFishDeth
{
	//   ������Ƿ������������ж�
public:
	DetectFishDeth(const vector<vector<Point> > &contours);
	~DetectFishDeth();
	//Get_relativepoz();


private:
	vector <Point>  center_point;
	vector <double>  speed;
	vector <int>  contour_area;
	int num_fish;


};

DetectFishDeth::DetectFishDeth(const vector<vector<Point> > &contours)
{
	num_fish = contours.size();
	
	for (int i = 0; i < num_fish; ++i)
	{
		//����������ĵ�
		Moments m=moments(contours[i]);
		center_point.push_back(Point(m.m10 / m.m00, m.m01 / m.m00));

		// �ٶȳ�ʼ��Ϊ0��
		speed.push_back(0);
		
		//�����ⲿ�������������
		contour_area.push_back(contourArea(contours[i]));
	}
}

DetectFishDeth::~DetectFishDeth()
{

}