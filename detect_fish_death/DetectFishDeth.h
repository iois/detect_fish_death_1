#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
class DetectFishDeth
{
	//   检测鱼是否死亡，概率判断
public:
	DetectFishDeth(const vector<vector<Point> > &contours);
	~DetectFishDeth();
	Get_relativepoz();


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
		Moments m=moments(contours[i]);
		center_point.push_back(Point(m.m10 / m.m00, m.m01 / m.m00));//计算鱼的中心点

		speed.push_back(0);// 速度初始化为0；
		
		contour_area.push_back(contourArea(contours[i]));//调用外部函数求轮廓面积
	}
}

DetectFishDeth::~DetectFishDeth()
{

}