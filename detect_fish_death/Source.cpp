#include <iostream>
#include "opencv2/opencv.hpp"
#include"DetectFishDeth.h"

using namespace std;
using namespace cv;

const int AVG_FISH_AREA = 700;
const int MIN_FISH_AREA = 500;
const int MAX_FISH_AREA = 1200;
const Scalar COLOR = Scalar(255, 255, 0);

int main()
{

	int num_fish = 0;
	cout << "输入鱼的条数：";
	cin >> num_fish;

	Mat src_gray;
	Mat dst;

	Mat background;
	background = imread("background.bmp", 0);//0:gray

	VideoCapture vidCapture("Video179.wmv");//Video179.wmv20140109.avitest_6fish.AVI
	Mat frame;

	namedWindow("Contours", CV_WINDOW_AUTOSIZE);

	DetectFishDeth *detect_fish_deth = nullptr;

	for (;;){
		vidCapture >> frame;

		cvtColor(frame, src_gray, COLOR_RGB2GRAY);

		absdiff(src_gray, background, dst);//差分

		bitwise_xor(Scalar(255, 0, 0, 0), dst, dst);//xor,颜色取反

		GaussianBlur(dst, dst, Size(5, 5), 0, 0);

		threshold(dst, dst, 200, 255, 0);//阈值分割

		imshow("camera 1 frame", dst);

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		findContours(dst, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		/*
		if (contours.size() < num_fish){
			cout << "输入的条数 少于 找到的目标数量" << endl;
			num_fish = contours.size();
			continue;
		}
		*/

		sort(contours.begin(), contours.end(), [](vector<Point> a, vector<Point>b)
		{
			return abs(contourArea(a) - AVG_FISH_AREA) < abs(contourArea(b) - AVG_FISH_AREA); 
		});

		vector<vector<Point> > another_contours;
		
		for (int j = 0; j < num_fish; ++j){
			if (j>contours.size()-1){
				another_contours.push_back(contours[0]);
			}
			else{
				another_contours.push_back(contours[j]);
			}
		}

		if (!detect_fish_deth){
			detect_fish_deth = new DetectFishDeth(another_contours, 600, 1000,num_fish);
		}
		else{
			vector<double> prob = detect_fish_deth->get_prob_death(another_contours);

			vector<double> speed = detect_fish_deth->get_speed();
			vector<double> area = detect_fish_deth->get_diff_area();
			vector<double> poz = detect_fish_deth->get_relativepoz();
			cout << "num of fishs: " << prob.size() << endl;
			for (int i = 0; i < prob.size(); ++i){
				cout << "prob=" << prob[i] << endl;
				cout << "speed= " << speed[i] << " area=" << area[i] << " poz= " << poz[i] <<  endl;
			}
			cout << endl;
		}

		/// Draw contours
		Mat drawing = Mat::zeros(dst.size(), CV_8UC3);
		for (int i = 0; i< contours.size(); i++)
		{
			//if (contours[i].size() >4)
			{
				drawContours(drawing, contours, i, COLOR, 2, 8, hierarchy, 0, Point());
				/*
				for (int j = 0; j < contours[i].size(); ++j){
				cout << contours[i][j].x << " " << contours[i][j].y << endl;
				}
				*/
			}
		}
		imshow("Contours", drawing);

		int key = waitKey(66);
		if (key == 27){ break; }
	}
	return 0;
}
